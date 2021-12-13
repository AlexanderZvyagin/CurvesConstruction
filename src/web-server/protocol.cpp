#include <libwebsockets.h>
#include <string.h>

#include <stdexcept>
#include <cmath>
#include <thread>
#include <future>
#include <list>
#include <memory>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <fmt/format.h>

#include "UserData.hpp"
#include "protocol.hpp"
#include "Session.hpp"
#include "log.hpp"

namespace {

const size_t data_chunk_size=4096;

struct HostData {
    struct lws_context *context;
    struct lws_vhost *vhost;
    const struct lws_protocols *protocol;
    std::list<std::shared_ptr<Session<UserData>>> *sessions;
    std::list<std::future<void>> *threads;
};

}

int
callback_protocol(struct lws *wsi, enum lws_callback_reasons reason,
            void *user, void *in, size_t len)
{
    logger->debug("callback function: START   len={}",len);

    HostData *host_data = static_cast<HostData *> (
        lws_protocol_vh_priv_get(
            lws_get_vhost(wsi),
            lws_get_protocol(wsi))
    );

    auto data = [host_data] (auto user) -> std::shared_ptr<Session<UserData>> {
        SessionWrapper *ud = static_cast<SessionWrapper*>(user);
        if( not (ud and ud->session) )
            throw std::runtime_error("Session is null");
        auto it = std::find_if(
            host_data->sessions->begin(),
            host_data->sessions->end(),
            [&ud] (auto x) {return x.get()==ud->session;}
        );
        if( it==host_data->sessions->end() )
            throw std::logic_error("internal error in callback_protocol: cannot find user session");
        return *it;
    };

    auto purge = [] (auto &threads) {
        for( auto it=threads.begin(); it!=threads.end(); )
            if( it->wait_for(std::chrono::seconds(0)) == std::future_status::ready )
                it = threads.erase(it);
            else
                it++;
    };

    try {
        switch (reason) {

            case LWS_CALLBACK_PROTOCOL_INIT: {
                logger->debug("LWS_CALLBACK_PROTOCOL_INIT");

                host_data = static_cast<HostData *>(
                    lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),
                        lws_get_protocol(wsi),
                        sizeof(struct HostData))
                );

                if (!host_data)
                    return -1;

                host_data->sessions = new std::list<std::shared_ptr<Session<UserData>>>();
                host_data->threads = new std::list<std::future<void>>();
                break;
            }

            case LWS_CALLBACK_PROTOCOL_DESTROY: {
                logger->debug("LWS_CALLBACK_PROTOCOL_DESTROY");
                if (!host_data)
                    break;
                delete host_data->sessions;
                delete host_data->threads;
                break;
            }

            case LWS_CALLBACK_EVENT_WAIT_CANCELLED: {
                lws_callback_on_writable_all_protocol(
                    lws_get_context(wsi),
                    lws_get_protocol(wsi)
                );
                break;
            }

            case LWS_CALLBACK_ESTABLISHED: {
                logger->debug("LWS_CALLBACK_ESTABLISHED");
                SessionWrapper *ud = static_cast<SessionWrapper*>(user);
                if( ud->session )
                    throw std::runtime_error("LWS_CALLBACK_ESTABLISHED: the data is not zero");
                ud->session = new Session<UserData>();
                ud->session->lws_ctx = lws_get_context(wsi);
                host_data->sessions->push_back( std::shared_ptr<Session<UserData>>(ud->session) );
                logger->debug("new session: {}",fmt::ptr(ud->session));

                break;
            }

            case LWS_CALLBACK_SERVER_WRITEABLE: {
                logger->debug("LWS_CALLBACK_SERVER_WRITEABLE");
                auto ud = data(user);

                if( ud->outbox.empty() )
                    break;

                auto &r = ud->outbox.front();

                char buf[LWS_PRE + data_chunk_size+1]; // 1 byte for the trailing 0
                size_t
                    fragment_size = std::min(data_chunk_size,r.reply_str.size()-r.reply_str_send_pos),
                    reply_str_send_pos__next = r.reply_str_send_pos + fragment_size;

                // copy the next fragment to the buffer
                memcpy(buf+LWS_PRE,r.reply_str.data()+r.reply_str_send_pos,fragment_size);
                buf[LWS_PRE+fragment_size]=0; // trailing zero!
                int
                    // bytes_to_send = lws_snprintf(buf+LWS_PRE,fragment_size,"%s",ud.reply_str.data()+ud.reply_str_send_pos),
                    is_start = !r.reply_str_send_pos,
                    is_end   = reply_str_send_pos__next==r.reply_str.size(),
                    flags = lws_write_ws_flags(LWS_WRITE_TEXT, is_start, is_end );

                if( int(fragment_size) != lws_write(wsi, reinterpret_cast<unsigned char*>(buf+LWS_PRE), fragment_size, static_cast<lws_write_protocol>(flags)) )
                    throw std::runtime_error("ERROR %d writing to di socket");

                if( is_end )
                    ud->outbox.erase(ud->outbox.begin());
                else {
                    r.reply_str_send_pos = reply_str_send_pos__next;
                    lws_callback_on_writable(wsi);
                }

                break;
            }

            case LWS_CALLBACK_RECEIVE: {
                logger->debug("LWS_CALLBACK_RECEIVE");

                if( len>=data_chunk_size )
                    throw std::runtime_error("Input data is too big");

                auto ud = data(user);
                json request = json::parse( std::string_view(static_cast<const char*>(in),len) );

                std::lock_guard<std::mutex> lock(ud->mtx);
                host_data->threads->emplace_back( std::async(
                    std::launch::async,
                    [request] (std::shared_ptr<Session<UserData>> ud) {
                        using namespace std::chrono_literals;
                        json rv = ud->data.Call(request);
                        std::lock_guard<std::mutex> lock(ud->mtx);
                        if( ud->lws_ctx ) {
                            ud->outbox.emplace_back(rv);
                            logger->debug("We have something to deliver! (thread notify)");
                            lws_cancel_service(ud->lws_ctx);
                        }
                    },
                    ud
                ));
                break;
            }

            case LWS_CALLBACK_CLOSED: {
                auto ud = data(user);
                std::lock_guard<std::mutex> lock(ud->mtx);
                ud->lws_ctx = nullptr;
                logger->debug("LWS_CALLBACK_CLOSED sessions={}",host_data->sessions->size());
                for( auto it=host_data->sessions->begin(); it!=host_data->sessions->end(); it++ )
                    if( it->get()==ud.get() ) {
                        host_data->sessions->erase(it);
                        break;
                    }
                purge(*host_data->threads);
                break;
            }

            default: {
                logger->warn("ignored reason={}",int(reason));
            }
        }

    } catch( const std::exception &e ) {
        logger->error(e.what());
        lws_close_reason(
            wsi,
            LWS_CLOSE_STATUS_GOINGAWAY,
            reinterpret_cast<unsigned char*> (const_cast<char*> (e.what()) ),
            strlen(e.what()) );
        return -1;
    }

    return 0;
}
