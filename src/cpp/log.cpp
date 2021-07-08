// #include "spdlog/sinks/rotating_file_sink.h"
#include <nlohmann/json.hpp>
#include "spdlog/sinks/systemd_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "log.hpp"

std::shared_ptr<spdlog::async_logger> logger;

std::shared_ptr<spdlog::async_logger> get_logger (std::string name) {
    fmt::print("get_logger({}) {}\n",name,fmt::ptr(logger.get()));
    return logger;
}


void logger_init (const std::string &config_json) {
    using json = nlohmann::json;
    auto cfg = json::parse(config_json);

    spdlog::init_thread_pool(8192, 1);

    std::string name = "mc";
    if( auto it=cfg.find("name"); it!=cfg.end() )
        name = *it;

    auto get_level = [] (const std::string &lvl) {

        auto tolower = [] (std::string s) {
            std::transform(s.begin(), s.end(), s.begin(),
                        // static_cast<int(*)(int)>(std::tolower)         // wrong
                        // [](int c){ return std::tolower(c); }           // wrong
                        // [](char c){ return std::tolower(c); }          // wrong
                           [](unsigned char c){ return std::tolower(c); } // correct
                          );
            return s;
        };

        if        (tolower(lvl)=="trace"){
            return spdlog::level::trace;
        } else if (tolower(lvl)=="debug"){
            return spdlog::level::debug;
        } else if (tolower(lvl)=="info"){
            return spdlog::level::info;
        } else if (tolower(lvl)=="warning"){
            return spdlog::level::warn;
        } else if (tolower(lvl)=="error"){
            return spdlog::level::err;
        } else if (tolower(lvl)=="critical"){
            return spdlog::level::critical;
        } else if (tolower(lvl)=="off"){
            return spdlog::level::off;
        } else {
            throw std::invalid_argument(fmt::format("logger_init: unknwon level '{}'",lvl));
        }
    };

    if( auto sinks=cfg.find("loggers"); sinks!=cfg.end()) {
        std::vector<spdlog::sink_ptr> vsinks;
        for( auto it=sinks->begin(); it!=sinks->end(); it++ ) {
            spdlog::sink_ptr sink;
            const auto type = (*it) ["type"];
            if        (type=="console") {
                sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            } else if ((type=="file")) {
                sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>((*it) ["path"]);
            } else if ((type=="spdlog")) {
                sink = std::make_shared<spdlog::sinks::systemd_sink_mt>();
            } else {
                throw std::invalid_argument(fmt::format("logger_init: unknwon sink type '{}'",type));
            }

            if( auto level=it->find("level"); level!=it->end() )
                sink->set_level(get_level(*level));
            vsinks.push_back(sink);
        }
        logger = std::make_shared<spdlog::async_logger>(name,vsinks.begin(),vsinks.end(),spdlog::thread_pool());
    }

    if( auto it=cfg.find("level"); it!=cfg.end() )
        logger -> set_level (get_level(*it));

    spdlog::flush_every(std::chrono::seconds(1));
    // spdlog::register_logger(logger);

    // return {};
}
