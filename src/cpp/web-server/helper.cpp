#include <set>
#include <fmt/format.h>
// #include <fmt/stream.h>
#include "helper.hpp"

namespace {

const LibWebSocketCallBackReason reasons_list [] = {{
        0, "LWS_CALLBACK_ESTABLISHED",
	    "(VH) after the server completes a handshake with an incoming "
	    "client.  If you built the library with ssl support, in is a "
	    "pointer to the ssl struct associated with the connection or NULL."
        "\n"
        "b0 of len is set if the connection was made using ws-over-h2"
    },{
	    6, "LWS_CALLBACK_RECEIVE",
	    "data has appeared for this server endpoint from a "
	    "remote client, it can be found at *in and is "
	    "len bytes long"
    },{
	    10, "LWS_CALLBACK_CLIENT_WRITEABLE",
	    "If you call lws_callback_on_writable() on a connection, you will "
	    "get one of these callbacks coming when the connection socket "
	    "is able to accept another write packet without blocking. "
	    "If it already was able to take another packet without blocking, "
	    "you'll get this callback at the next call to the service loop "
	    "function.  Notice that CLIENTs get LWS_CALLBACK_CLIENT_WRITEABLE "
	    "and servers get LWS_CALLBACK_SERVER_WRITEABLE."
    },{
        11, "LWS_CALLBACK_SERVER_WRITEABLE",
	    "See LWS_CALLBACK_CLIENT_WRITEABLE"
    },{
	    20, "LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION",
        "called when the handshake has "
	    "been received and parsed from the client, but the response is "
	    "not sent yet.  Return non-zero to disallow the connection. "
	    "user is a pointer to the connection user space allocation, "
	    "in is the requested protocol name "
	    "In your handler you can use the public APIs "
	    "lws_hdr_total_length() / lws_hdr_copy() to access all of the "
	    "headers using the header enums lws_token_indexes from "
	    "libwebsockets.h to check for and read the supported header "
	    "presence and content before deciding to allow the handshake "
	    "to proceed or to kill the connection."
    },{
        27, "LWS_CALLBACK_PROTOCOL_INIT",
        " One-time call per protocol, per-vhost using it, so it can "
        " do initial setup / allocations etc "
    },{
        28, "LWS_CALLBACK_PROTOCOL_DESTROY",
        " One-time call per protocol, per-vhost using it, indicating "
        "this protocol won't get used at all after this callback, the "
        "vhost is getting destroyed.  Take the opportunity to "
        "deallocate everything that was allocated by the protocol. "
    },{
	    49, "LWS_CALLBACK_HTTP_BIND_PROTOCOL",
	    "By default, all HTTP handling is done in protocols[0]. "
	    "However you can bind different protocols (by name) to "
	    "different parts of the URL space using callback mounts.  This "
	    "callback occurs in the new protocol when a wsi is bound "
	    "to that protocol.  Any protocol allocation related to the "
	    "http transaction processing should be created then. "
	    "These specific callbacks are necessary because with HTTP/1.1, "
	    "a single connection may perform at series of different "
	    "transactions at different URLs, thus the lifetime of the "
	    "protocol bind is just for one transaction, not connection."
    },{
        53, "LWS_CALLBACK_ADD_HEADERS",
	    "This gives your user code a chance to add headers to a server"
	    "transaction bound to your protocol.  `in` points to a "
	    "`struct lws_process_html_args` describing a buffer and length "
	    "you can add headers into using the normal lws apis. "
        "\n"
	    "(see LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER to add headers to "
	    "a client transaction) "
	    "\n"
	    "Only `args->p` and `args->len` are valid, and `args->p` should "
	    "be moved on by the amount of bytes written, if any.  Eg "
	    "\n"
	    "    case LWS_CALLBACK_ADD_HEADERS:\n"
	    "\n"
	    "    struct lws_process_html_args *args =\n"
        "    (struct lws_process_html_args *)in;\n"
	    "\n"
	    "    if (lws_add_http_header_by_name(wsi,\n"
	    "       (unsigned char *)\"set-cookie:\",\n"
	    "       (unsigned char *)cookie, cookie_len,\n"
	    "       (unsigned char **)&args->p,\n"
        "       (unsigned char *)args->p + args->max_len))\n"
	    "       return 1;\n"
        "\n"
	    "break\n;"
    },{
        71, "LWS_CALLBACK_EVENT_WAIT_CANCELLED",
        "This is sent to every protocol of every vhost in response "
        "to lws_cancel_service() or lws_cancel_service_pt().  This "
        "callback is serialized in the lws event loop normally, even "
        "if the lws_cancel_service[_pt]() call was from a different "
        "thread."
    },{
        -1,nullptr,nullptr
}};

} // namespace

const LibWebSocketCallBackReason & lws_cb_reason(int number) {
    static LibWebSocketCallBackReason rv {0,"undocumented",""};
    for( const LibWebSocketCallBackReason *x=reasons_list; x->short_description!=nullptr; x++ )
        if( x->number == number )
            return *x;

    rv.number = number;
    return rv;
}

// void lws_print_reason(int reason) {
//     auto r = get_reason(reason);
//     // auto bg = fmt::bg(fmt::color::black);
//     fmt::print(
//         fmt::emphasis::bold | fmt::fg(fmt::color::green),
//         "[{}]  ", r.number);
//     fmt::print(
//         fmt::fg(fmt::color::green),
//         "{}",r.short_description);
//     fmt::print(" ...\n");
//     fmt::print(
//         fmt::fg(fmt::color::yellow),
//         "... {}\n\n", r.long_description);
// }

std::ostream & operator << (std::ostream &os, const LibWebSocketCallBackReason &r){

    os << fmt::format(
        // fmt::emphasis::bold | fmt::fg(fmt::color::green),
        "[{}]  ", r.number);
    os << fmt::format(
        // fmt::fg(fmt::color::green),
        "{}",r.short_description);
    os << " ...\n";
    os << fmt::format(
        // fmt::fg(fmt::color::yellow),
        "... {}\n\n", r.long_description);
    return os;
}
