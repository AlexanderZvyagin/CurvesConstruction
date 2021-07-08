#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <fmt/ostream.h>

#include "DB.hpp"
#include "log.hpp"
#include "utils.hpp"

using namespace date::literals;
using namespace std::chrono_literals;

TEST_CASE("get"){
    nlohmann::json cfg = {
        {"name","DB"},
        {"loggers", {
            {{"type","console"}},
        }}
    };
    logger_init(cfg.dump());

    DB db("mongodb://localhost:27017","MarketData","b20190612");
    date::year_month_day the_date {2018_y/sep/3};
    auto data = db.LoadData(the_date);
    fmt::print("On {} we have {} instruments.\n",to_str(the_date),data.size());

    for(auto [name,value]: data)
        fmt::print("{} {}\n",name,value);
}
