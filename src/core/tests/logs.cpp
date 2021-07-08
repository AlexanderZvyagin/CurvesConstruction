#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <nlohmann/json.hpp>
#include "log.hpp"

TEST_CASE("default"){
    nlohmann::json cfg = {
        {"name",""}, // The default logger has no name
        {"loggers", {
            {{"type","console"}},
            {{"type","file"},{"path","log-info.txt"},{"level","INFO"}},
            {{"type","file"},{"path","log-warn.txt"},{"level","WARNING"}}
        }}
    };
    logger_init(cfg.dump());

    debug("debug");
    info("info");
    warning("warning");
    error("error");
    info("{}",cfg.dump(4));
}
