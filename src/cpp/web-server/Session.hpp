#pragma once

#include <string>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>

struct lws_context;

template <typename T>
class Session {
public:
    struct Reply {
        Reply(const nlohmann::json &x) : reply_str(x.dump()) {}
        std::string reply_str;
        size_t reply_str_send_pos {0};
    };

    Session(void) {}
    Session(const Session &) = delete;
    Session(Session &&) = delete;
    ~Session(void) {}
    Session & operator = (const Session &) = delete;
    Session & operator = (Session &&) = delete;


    std::vector<Reply> outbox;
    struct lws_context *lws_ctx {nullptr};
    std::mutex mtx;

    T data;
};
