#include <sstream>
#include <fmt/ostream.h>

#include "UserData.hpp"
#include "log.hpp"
#include "DB.hpp"

#include "YieldCurve.hpp"
#include "ForwardRateAgreement.hpp"

using json = nlohmann::json;
using namespace date::literals;
using namespace std::chrono_literals;

// namespace {
// auto logger = get_logger("UserData");
// }

UserData::UserData(void) {
    logger->debug("UserData Ctor");
    // gsl_set_error_handler_off();
}

UserData::~UserData(void) {
    logger->debug("UserData Dtor");
}

json UserData::Call(const json &data ) noexcept {
    static const char
        *payload = "payload",
        *request_id = "request_id";
    json rv, pld;
    std::string type;

    try {
        logger->debug("{} {}",__PRETTY_FUNCTION__,data.dump(4));

        type = data.at("type").get<std::string>();
        pld[request_id] = data.at(payload).at(request_id).get<int>();

        if( type == "echo" ) {
            pld = data.at(payload);
        } else if( type == "get_quotes" ) {
            DB db("mongodb://localhost:27017","MarketData","b20190612");
            const std::string the_date_str = data.at(payload).at("date");
            date::year_month_day the_date;
            std::istringstream(the_date_str) >> date::parse("%F",the_date);
            // date::year_month_day the_date {2018_y/sep/3};
            auto db_data = db.LoadData(the_date);
            logger->debug("On {} we have {} instruments.\n",to_str(the_date),db_data.size());

            pld["date"] = the_date_str;
            json &quotes = pld["quotes"];
            for(auto &[name,value]: db_data){
                // json instrument_type;
                // try {
                //     auto r = decode_FRA_periods(name);
                //     instrument_type = json {{"type","FRA"},{"start",r.first},{"length",r.second}};
                //     logger->info("{}",instrument_type.dump());
                // }catch(const std::exception &e){
                //     logger->warn("{}",e.what());
                // }
                quotes.push_back(json{name,value});
            }
        } else if( type == "build_curve" ) {
            YieldCurve curve;

            const std::string the_date_str = data.at(payload).at("date");
            date::year_month_day the_date;
            std::istringstream(the_date_str) >> date::parse("%F",the_date);
            auto instruments = data.at(payload).at("instruments");
            for(auto instr: instruments){
                auto type = instr.at("type").get<std::string>();
                if(type=="FRA") {
                    int
                        start  = instr.at("start" ).get<int>(),
                        length = instr.at("length").get<int>();
                    double
                        quote = instr.at("quote").get<double>();
                    curve.Add(ForwardRateAgreement(start/12.,length/12.,quote));
                }else{
                    debug("skipping: {}",instr.dump());
                }
                // debug("{}",instr.dump());
            }

            curve.Build();
            curve.Print();

            auto points = data.at(payload).at("points").get<int>();

            if(curve.GetX().empty())
                throw std::runtime_error("Failed to build the curve");
            auto tmax = curve.GetX().back();

            json vx,vy;
            for(int i=0; i<points; i++){
                auto t = (tmax*i)/(points-1);
                vx.push_back(t);
                vy.push_back(curve(t));
            }

            pld["plot"] = json {{"x",vx},{"y",vy}};

        } else {
            throw std::invalid_argument("The request is not implemented by the C++ engine.");
        }

        // Normal return
        rv = {
            {"type",type+"_OK"}
        };

        if( not pld.is_null() )
            rv[payload] = std::move(pld);

    } catch( const std::exception &e ) {
        logger->error("{}",e.what());
        // Error return
        rv = json({
            {"type",type+"_ERROR"},
            {"error",true},
            {payload,{
                {"name",e.what()}
            }}
        });
    }

    return rv;
}
