#include <sstream>
#include <fmt/ostream.h>

#include "UserData.hpp"
#include "log.hpp"
#include "DB.hpp"

#include "YieldCurve.hpp"
#include "ForwardRateAgreement.hpp"
#include "Swap.hpp"

using json = nlohmann::json;
using namespace date::literals;
using namespace std::chrono_literals;

namespace {
const char
    *payload = "payload",
    *request_id = "request_id";
}

UserData::UserData(void) {
    logger->debug("UserData Ctor");
}

UserData::~UserData(void) {
    logger->debug("UserData Dtor");
}

json UserData::Call(const json &data ) noexcept {
    json rv, pld;
    std::string type;

    try {
        logger->debug("{} {}",__PRETTY_FUNCTION__,data.dump(4));

        type = data.at("type").get<std::string>();
        pld[request_id] = data.at(payload).at(request_id).get<int>();

        if( type == "echo" ) {
            pld = data.at(payload);
        } else if( type == "get_quotes" ) {
            get_quotes(data,pld);
        } else if( type == "build_curve" ) {
            build_curve(data,pld);
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

void UserData::get_quotes (const json &data, json &pld) {
    // connect to the DB
    // FIXME:config: constants in the code
    DB db("mongodb://localhost:27017","MarketData","b20190612");

    // get the date, return the date
    const std::string the_date_str = data.at(payload).at("date");
    date::year_month_day the_date;
    std::istringstream(the_date_str) >> date::parse("%F",the_date);
    pld["date"] = the_date_str;

    // get quotes from the db
    auto db_data = db.LoadData(the_date);
    logger->debug("On {} we have {} instruments.\n",to_str(the_date),db_data.size());

    // store quotes
    json &quotes = pld["quotes"];
    for(auto &[name,value]: db_data)
        quotes.push_back(json{name,value});
}

Swap create_swap(
    float fair_rate,
    YieldCurve &float_curve,
    float maturity,
    float dt1,
    float dt2
){
    Swap swap;
    const float t0 {0};

    LegFixed &lfix = swap.lfix;
    lfix.t0 = t0;
    lfix.dt = dt1;
    lfix.n = std::round(maturity/dt1);
    lfix.rate = fair_rate;

    LegFloat &lflt = swap.lflt;
    lflt.t0 = t0;
    lflt.dt = dt2;
    lflt.n  = std::round(maturity/dt2);
    lflt.curve = &float_curve;

    return swap;
}


void UserData::build_curve (const json &data, json &pld) {
    YieldCurve curve;

    const std::string the_date_str = data.at(payload).at("date");
    date::year_month_day the_date;
    std::istringstream(the_date_str) >> date::parse("%F",the_date);
    auto instruments = data.at(payload).at("instruments");
    const std::string interpolation_type = data.at(payload).at("interpolation_type");

    for(auto instr: instruments){
        auto type = instr.at("type").get<std::string>();
        if(type=="FRA") {
            int
                start  = instr.at("start" ).get<int>(),
                length = instr.at("length").get<int>();
            double
                quote = instr.at("quote").get<double>();
            curve.Add(ForwardRateAgreement(start/12.,length/12.,quote));
        } else if(type=="Swap") {
            int
                length = instr.at("length").get<int>(),
                leg1   = instr.at("leg1"  ).get<int>(),
                leg2   = instr.at("leg2"  ).get<int>();
            double
                quote = instr.at("quote").get<double>();
            curve.Add(create_swap(quote,curve,length/12.,leg1/12.,leg2/12.));
        }else{
            debug("skipping: {}",instr.dump());
        }
        // debug("{}",instr.dump());
    }

    auto result = curve.Build(YieldCurve::GetType(interpolation_type));
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

    if(1){
        json result_js = {
            {"value",           result.value},
            {"value_error",     result.error},
            {"calls",           result.calls},
            {"interpolation",   {
                {"type",curve.GetTypeName()},
                {"x",   nullptr},
                {"y",   nullptr}
            }}
        };
        if(!result.GetError().empty())
            result_js["error"] = result.GetError();
        json
            &js_x = result_js["interpolation"]["x"],
            &js_y = result_js["interpolation"]["y"];
        const auto
            vx = curve.GetX(),
            vy = curve.GetY();
        for(int i=0;i<curve.GetSize();i++){
            js_x.push_back(vx.at(i));
            js_y.push_back(vy.at(i));
        }

        json js_instr;
        for(auto [t,instr]: curve.GetInstruments() ){
            json jj {
                {"about",instr->About()},
                {"quote",instr->Value()},
                {"computed",instr->Eval(curve)},
            };
            if(auto p=dynamic_cast<ForwardRateAgreement*>(instr.get());p){
                jj["type"  ] = "FRA";
                jj["start" ] = p->start;
                jj["length"] = p->length;
            }
            if(auto p=dynamic_cast<Swap*>(instr.get());p){
                jj["type"  ] = "Swap";
                jj["length"] = std::max(p->lfix.dt*p->lfix.n,p->lflt.dt*p->lflt.n);
                jj["leg1"] = p->lfix.dt;
                jj["leg2"] = p->lflt.dt;
            }
            js_instr.push_back(std::move(jj));
        }
        result_js["instruments"] = std::move(js_instr);
        pld["result"] = std::move(result_js);
    }
}
