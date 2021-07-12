/*
 * lws-minimal-ws-server
 *
 * Copyright (C) 2018 Andy Green <andy@warmcat.com>
 *
 * This file is made available under the Creative Commons CC0 1.0
 * Universal Public Domain Dedication.
 *
 * This demonstrates the most minimal http server you can make with lws,
 * with an added websocket chat server.
 *
 * To keep it simple, it serves stuff in the subdirectory "./mount-origin" of
 * the directory it was started in.
 * You can change that by changing mount.origin.
 */

#include <libwebsockets.h>
#include <string.h>
#include <csignal>
#include <nlohmann/json.hpp>

#include "spdlog/sinks/basic_file_sink.h"

#include "fmt/ostream.h"
#include "clara.hpp"
#include "protocol.hpp"
#include "log.hpp"
// #include "FuncTime.hh"

namespace {
bool interrupted {false};
// auto logger = get_logger("server");
}

void sigint_handler(int sig) {
    logger->debug("signal {}",sig);
    if(interrupted) {
        logger->warn("got the signal more then once, forcing exit");
        exit(0);
    }
    interrupted = true;
}

int main(int argc, const char **argv) {

    nlohmann::json cfg = {
        {"name","ws"}, // The default logger has no name
        {"level","DEBUG"},
        {"loggers", {
            {{"type","console"},{"level","DEBUG"}},
            {{"type","file"},{"path","log-info.txt"},{"level","INFO"}},
            {{"type","file"},{"path","log-warn.txt"},{"level","WARNING"}}
        }}
    };
    logger_init(cfg.dump());
    debug("debug test");

    // FuncTime ft(__PRETTY_FUNCTION__);
    bool help {false};

    // std::string
    // 	mount_dir,
    // 	file_default="index.html";

    struct lws_http_mount mount = {
        /* .mount_next */		NULL,		/* linked-list "next" */
        /* .mountpoint */		"/",		/* mountpoint URL */
        /* .origin */			0,  	/* serve from dir */
        /* .def */			    "index.html",	/* default filename */
        /* .protocol */			NULL,
        /* .cgienv */			NULL,
        /* .extra_mimetypes */		NULL,
        /* .interpret */		NULL,
        /* .cgi_timeout */		0,
        /* .cache_max_age */		0,
        /* .auth_mask */		0,
        /* .cache_reusable */		0,
        /* .cache_revalidate */		0,
        /* .cache_intermediaries */	0,
        /* .origin_protocol */		LWSMPRO_FILE,	/* files in a dir */
        /* .mountpoint_len */		1,		/* char count */
        /* .basic_auth_login_file */	NULL,
        /* unused */ 0
    };

    // struct lws_protocol_vhost_options:
    // 	 	const struct lws_protocol_vhost_options *next; /**< linked list */
    // 		const struct lws_protocol_vhost_options *options; /**< child linked-list of more options for this node */
    // 		const char *name; /**< name of name=value pair */
    // 		const char *value; /**< value of name=value pair */

    // struct lws_protocol_vhost_options pvo_hsbph[] = {{
    //     NULL, NULL, "referrer-policy:", "no-referrer"
    // }, {
    //     &pvo_hsbph[0], NULL, "x-frame-options:", "deny"
    // }, {
    //     &pvo_hsbph[1], NULL, "x-xss-protection:", "1; mode=block"
    // }, {
    //     &pvo_hsbph[2], NULL, "x-content-type-options:", "nosniff"
    // }, {
    //     &pvo_hsbph[3], NULL, "content-security-policy:",
    //     "default-src 'none'; img-src 'self' data: ; "
    //         "script-src 'self'; font-src 'self'; "
    //         "style-src 'self'; connect-src 'self'; "
    //         "frame-ancestors 'none'; base-uri 'none';"
    //         "form-action 'self';"
    // }};

    // const struct lws_protocol_vhost_options vhost_options[] = {{
    //     vhost_options+1,
    //     nullptr,
    //     "referrer-policy:",
    //     "no-referrer"
    // },{
    //     vhost_options+2,
    //     nullptr,
    //     "x-frame-options:",
    //     "deny"
    // },{
    //     vhost_options+3,
    //     nullptr,
    //     "x-xss-protection:",
    //     "1; mode=block"
    // },{
    //     nullptr,
    //     nullptr,
    //     "x-content-type-options:",
    //     "nosniff"
    // }};

    // if(all_logs)
    //     for( auto name: *all_logs )
    //         get_logger(name)->set_level(spdlog::level::debug);
    // get_logger("bermud-protocol")->set_level(spdlog::level::info);
    // get_logger("DB")->set_level(spdlog::level::info);
    // get_logger("AllData")->sinks().push_back(
    //     std::make_shared<spdlog::sinks::basic_file_sink_mt>("AllData.log"));


    struct lws_context_creation_info info;
    struct lws_context *context;
    // const char *p;
    int n = 0, logs = LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE
            /* for LLL_ verbosity above NOTICE to be built into lws,
             * lws must have been configured and built with
             * -DCMAKE_BUILD_TYPE=DEBUG instead of =RELEASE */
            /* | LLL_INFO */ /* | LLL_PARSER */ /* | LLL_HEADER */
            /* | LLL_EXT */ /* | LLL_CLIENT */ /* | LLL_LATENCY */
            /* | LLL_DEBUG */;

    signal(SIGINT, sigint_handler);

    // if ((p = lws_cmdline_option(argc, argv, "-d")))
    // 	logs = atoi(p);

    lws_set_log_level(logs, NULL);
    // lwsl_user("LWS minimal ws server | visit http://localhost:7681 (-s = use TLS / https)\n");

    memset(&info, 0, sizeof info); /* otherwise uninitialized garbage */
    //info.headers = vhost_options;

    size_t buffer_size = 8192;
    info.port = 8100;

    std::string mount_origin, file_default {"index.html"};

    if(1) {
        auto cli
            = clara::Help( help )
            | clara::Opt( info.port, "port" )
                ["--port"]
                (fmt::format("The server port [default={}]",info.port))
            | clara::Opt( buffer_size, "buffer" )
                ["--buffer"]
                (fmt::format("Communication buffer size, bytes [default={}]",buffer_size))
            | clara::Opt( file_default, "file-default" )
                ["--file-default"]
                (fmt::format("Default file to serve [default={}]",file_default))
            // | clara::Opt( force_curve_build )
            //     ["--build-curves"]
            //     ("Force curves construction, even if the DB has curves already")
            | clara::Arg( mount_origin, "dir" )
                ("Directory with the files to serve (like 'index.html')")
        ;

        if( auto result = cli.parse( clara::Args( argc, argv ) ); !result )
            throw std::runtime_error(result.errorMessage());

        if( help or mount_origin.empty() ) {
            fmt::print("{}\n",cli);
            return 1;
        }
    }

    mount.origin = mount_origin.c_str();

    logger->info("port={} origin={}\n",info.port,mount.origin);

    struct lws_protocols protocols[] = {
        { "http", lws_callback_http_dummy, 0, 0, 0, NULL, 0 },
        {
            "curves",
            callback_protocol,
            sizeof(struct SessionWrapper),
            buffer_size,
            0, NULL, 0
        },
        { NULL, NULL, 0, 0, 0, NULL, 0 } /* terminator */
    };

    info.mounts = &mount;
    info.protocols = protocols;
    info.vhost_name = "localhost";
    // info.ws_ping_pong_interval = 10;
    // info.options = LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;

    // if (lws_cmdline_option(argc, argv, "-s")) {
    // 	lwsl_user("Server using TLS\n");
    // 	info.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
    // 	info.ssl_cert_filepath = "localhost-100y.cert";
    // 	info.ssl_private_key_filepath = "localhost-100y.key";
    // }

    // if (lws_cmdline_option(argc, argv, "-h"))
    // 	info.options |= LWS_SERVER_OPTION_VHOST_UPG_STRICT_HOST_CHECK;


    context = lws_create_context(&info);
    if (!context) {
        lwsl_err("lws init failed\n");
        return 1;
    }

    while (n >= 0 && !interrupted)
        n = lws_service(context, 1000);

    lws_context_destroy(context);

    return 0;
}
