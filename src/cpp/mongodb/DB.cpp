#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/decimal128.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

// #include <ql/time/daycounters/all.hpp>

#include <iterator>
#include <regex>
#include <unordered_map>
#include <chrono>
#include <iostream>
#include <fmt/ostream.h>

#include "DB.hpp"
#include "log.hpp"
// #include "api.hh"
// #include "Swaption.hh"
// #include "SABR/SABR.hh"

using date::operator<<;
using nlohmann::json;

std::recursive_mutex db_mutex;

namespace {
// auto logger = get_logger("DB");
mongocxx::instance inst {};

template<class T>
std::string
get_string(const T &x) {
    if( x and x.type()==bsoncxx::type::k_utf8 )
        return x.get_utf8().value.to_string();
    throw std::invalid_argument("get_string()");
}

template<class T>
double
get_double(const T &x) {
    if( x ) {
        switch( x.type() ) {
            case bsoncxx::type::k_double:       return x.get_double();
            case bsoncxx::type::k_decimal128:   return std::stod(x.get_decimal128().value.to_string());
            case bsoncxx::type::k_int32:        return x.get_int32();
            case bsoncxx::type::k_int64:        return x.get_int64();
            default: {}
        }
    }
    throw std::invalid_argument("get_double()");
}

template<class T>
date::year_month_day
get_year_month_day(const T &x) {
    if( x and x.type()==bsoncxx::type::k_date )
        // return std::chrono::floor<date::days>(x.get_date());
        return std::chrono::floor<date::days>(std::chrono::system_clock::time_point(x.get_date()));
    throw std::invalid_argument("get_year_month_day()");
}

#if 0
std::unique_ptr<ql::DayCounter>
dcc_from_name(std::string_view name) {
    std::unique_ptr<ql::DayCounter> p;

    p = std::make_unique<ql::Actual360>();
    if( p->name() == name )
        return p;

    p = std::make_unique<ql::Actual365Fixed>();
    if( p->name() == name )
        return p;

    p = std::make_unique<ql::ActualActual>();
    if( p->name() == name )
        return p;

    p = std::make_unique<ql::SimpleDayCounter>();
    if( p->name() == name )
        return p;

    p = std::make_unique<ql::Thirty360>();
    if( p->name() == name )
        return p;

    throw std::invalid_argument("dcc_from_name");
}
#endif

}


json
load_json (
    mongocxx::collection col,
    const json &j,
    bool find_one
);

void
save_json (
    mongocxx::collection col,
    const json &doc,
    const json &filter
);

DB::DB(const std::string &db_server,const std::string &db_name, const std::string &quotes_collection) :
    db_server( db_server ),
    db_name( db_name ),
    quotes_collection( quotes_collection ),
    conn( mongocxx::uri(db_server) )
{}

MarketData DB::LoadData(date::year_month_day d) {

    std::lock_guard<std::recursive_mutex> lock(db_mutex);
    auto coll = conn [db_name] [quotes_collection+".instruments"];

    mongocxx::cursor cursor = coll.find(
        bsoncxx::builder::stream::document {}
            << "date"
            << bsoncxx::types::b_date(date::sys_days(d))
            << bsoncxx::builder::stream::finalize
    );

    MarketData data;
    for(auto doc : cursor)
        data.insert( std::make_pair(
            get_string(doc["name"]),
            get_double(doc["quote"])
        ));

    logger->debug("LoadData(): for {} returning {} instruments",d,data.size());

    return data;
}

#if 0

std::vector<Swaption> DB::LoadSwaptionsStructure(const std::string &swaptions_index ) {

    std::vector<Swaption> swaptions;

    std::lock_guard<std::recursive_mutex> lock(db_mutex);
    auto coll = conn [db_name] [quotes_collection+".swaptions"];

    mongocxx::cursor cursor = coll.find(
        bsoncxx::builder::stream::document {}
            << "index"
            << bsoncxx::types::b_utf8(swaptions_index)
            << bsoncxx::builder::stream::finalize
    );

    // {   "index" : "Euribor6M",
    //     "option_expiry" : 9,
    //     "swap_tenor" : 24,
    //     "currency" : "EUR",
    //     "points" : {
    //         "0" : {
    //             "name" : "EUPEI2",
    //             "source" : "TRPU"
    //         }
    //     }
    // }

    std::for_each( cursor.begin(), cursor.end(), [&swaptions] (auto &&doc) {
        Swaption sw (
            doc ["option_expiry"] .get_int32(),
            doc ["swap_tenor"]    .get_int32()
        );

        auto points = doc ["points"].get_document().view();

        std::for_each( points.cbegin(), points.cend(), [&sw,&points,&swaptions] (auto &x) {
            std::string key = x.key().to_string();
            int bp = std::stoi( key );
            sw.points [bp] = points [key] ["name"] .get_utf8() .value .to_string();
        });

        swaptions.push_back(std::move(sw));
    });

    return swaptions;
}

void DB::LoadSwaptions (date::year_month_day d, std::vector<Swaption> &swaptions ) {

    std::lock_guard<std::recursive_mutex> lock(db_mutex);
    auto coll = conn [db_name] [quotes_collection+".instruments"];

    // key: instrument name. data: Swaption and offset
    std::unordered_map<std::string,std::pair<Swaption*,int>> refs;

    auto set_swaption_quote = [&refs] (const std::string &name,double quote) {
        if( quote>0 ) {
            // logger->debug("    {} = {}",name,quote);
            auto [sw,offset] = refs.at(name);
            size_t i=0;
            while( i<sw->offsets.size() && sw->offsets[i]<offset )
                i++;

            auto it1 = sw->offsets.begin();
            std::advance(it1,i);
            sw->offsets.insert(it1,offset);

            auto it2 = sw->quotes.begin();
            std::advance(it2,i);
            sw->quotes.insert(it2,quote);
        }
        else
            logger->warn("non-positive swaption quote: {} {}",name,quote);
    };

    bsoncxx::builder::stream::array names {};
    for( auto &sw: swaptions ) {
        for( auto &[offset,name]: sw.points ) {
            names << name;
            refs[name] = std::make_pair(&sw,offset);
            sw.ymd = d;
        }
    }

    bsoncxx::document::value b1 = bsoncxx::builder::stream::document {}
        << "date" << bsoncxx::types::b_date(date::sys_days(d))
        << "name" << bsoncxx::builder::stream::open_document
            << "$in" << names.view()
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    mongocxx::cursor cursor = coll.find(std::move(b1));

    std::for_each(cursor.begin(),cursor.end(), [&set_swaption_quote] (auto &&doc) {
        auto name = doc["name"];
        auto quote = doc["quote"];
        if( name and quote )
            set_swaption_quote(get_string(name),get_double(quote));
        else
            logger->error("Bad DB record");
    });
}

using namespace bsoncxx::builder::stream;

template<class CurveType>
array
curve_points (const ql::YieldTermStructure *ycurve) {

    const CurveType *curve = dynamic_cast<const CurveType*>(ycurve);
    if(not curve)
        throw std::logic_error("curve_points(): internal error");

    array data{};
    for( auto [t,df]: curve->nodes() ) {
        data << open_array
            << bsoncxx::types::b_date (
                date::sys_days {
                    date::year(t.year()) /
                    date::month(static_cast<unsigned>(t.month())) /
                    date::day(t.dayOfMonth())} )
            << df
        << close_array;
    }
    return data;
}

void
DB::SaveCurves (
    const Curves &curves,
    date::year_month_day ymd,
    const std::vector<std::string> &tags
){
    logger->info("save_curves is called for {}",ymd);

    std::lock(db_mutex, ql_mutex);
    std::lock_guard<std::recursive_mutex> db_lock(db_mutex,std::adopt_lock);
    std::lock_guard<std::recursive_mutex> ql_lock(ql_mutex,std::adopt_lock);

    // #pragma message "FIXME: constant in the code"
    auto curves_collection = conn [db_name] ["curves"];

    auto curve_discount = curves.first;
    auto curve_index = curves.second;

    auto tags_array = [] (auto tags) {
        array a{};
        for( auto tag: tags )
            a << tag;
        return a;
    };

    auto doc = document{}
        << "date"   << bsoncxx::types::b_date(date::sys_days(ymd))
        << "tags" << tags_array(tags)
        << "curves" << open_array
            << open_document
                << "name" << "discount"
                << "day_counting_convention" << curve_discount->dayCounter().name()
                << "data" << curve_points<qlCurveDiscount>(curve_discount.get())
            << close_document
            << open_document
                << "name" << "index"
                << "day_counting_convention" << curve_index->dayCounter().name()
                << "data" << curve_points<qlCurveIndex>(curve_index.get())
            << close_document
        << close_array
        << finalize;

    curves_collection .find_one_and_replace (

        // filter
        document{}
            << "date"   << bsoncxx::types::b_date(date::sys_days(ymd))
            << finalize,

        // document to upsert
        std::move(doc),

        mongocxx::options::find_one_and_replace() .upsert(true)
    );
}

std::optional<Curves>
DB::LoadCurves (
    date::year_month_day ymd,
    const std::vector<std::string> &tags
){
    if( not tags.empty() )
        throw std::invalid_argument("load_curves(): non-empty tags are not supported yet");

    std::lock(db_mutex, ql_mutex);
    std::lock_guard<std::recursive_mutex> db_lock(db_mutex,std::adopt_lock);
    std::lock_guard<std::recursive_mutex> ql_lock(ql_mutex,std::adopt_lock);

    //! FIXME: constant in the code
    auto doc = conn [db_name] ["curves"] .find_one (
        bsoncxx::builder::stream::document {}
            << "date"   << bsoncxx::types::b_date(date::sys_days(ymd))
            << bsoncxx::builder::stream::finalize
    );

    if( not doc )
        return {};

    logger->debug("found curves at {}",ymd);

    std::unordered_map< std::string, std::vector< ql::Date > > map_dates;
    std::unordered_map< std::string, std::vector< ql::Rate > > map_data;
    std::unordered_map< std::string, std::unique_ptr<ql::DayCounter> > map_dcc;

    auto curves_array_element = doc .value().view() ["curves"];
    if( not (curves_array_element and curves_array_element.type() == bsoncxx::type::k_array) )
        throw std::runtime_error("load_curves: cannot find 'curves' array");

    bsoncxx::array::view curves_array {curves_array_element.get_array().value};
    for (bsoncxx::array::element curve_item : curves_array) {

        if (curve_item.type() != bsoncxx::type::k_document)
            throw std::runtime_error("load_curves: wrong curve type");

        bsoncxx::document::view curve {curve_item.get_document()};
        auto curve_name = get_string(curve ["name"]);
        auto curve_data = curve ["data"];

        if( not (curve_data and curve_data.type()==bsoncxx::type::k_array) )
            throw std::runtime_error("load_curves: bad curve 'data' type");

        logger->debug("load_curves: curve '{}'",curve_name);

        auto &dates = map_dates[curve_name];
        auto &data  = map_data [curve_name];
        map_dcc[curve_name] = dcc_from_name(get_string(curve ["day_counting_convention"]));

        bsoncxx::array::view data_array {curve_data.get_array().value};
        for (bsoncxx::array::element data_item : data_array) {

            if (data_item.type() != bsoncxx::type::k_array)
                throw std::runtime_error("load_curves: wrong data item type");

            dates.push_back( to_qlDate(get_year_month_day(data_item[0])) );
            data .push_back( get_double(data_item[1]) );
        }
    }

    Curves curves{
        boost::shared_ptr<qlCurveDiscount>(
            new qlCurveDiscount (
                map_dates.at(curve_discount_name),
                map_data .at(curve_discount_name),
                *map_dcc .at(curve_discount_name)
        )),
        boost::shared_ptr<qlCurveIndex>(
            new qlCurveIndex (
                map_dates.at(curve_index_name),
                map_data .at(curve_index_name),
                *map_dcc .at(curve_index_name)
        ))
    };

    curves.first  -> enableExtrapolation(true);
    curves.second -> enableExtrapolation(true);

    return curves;
}
#endif

using bsoncxx::builder::basic::kvp;

void
append(
    bsoncxx::builder::basic::document &x,
    const std::string &k,
    const json &v);

void
append(
    bsoncxx::builder::basic::array &x,
    const json &v
){
    if( v.is_object() ) {
        bsoncxx::builder::basic::document doc;
        for( auto &[key,value]: v.items() )
            append(doc,key,value);
        x.append(doc.extract().view());
    } else
    if( v.is_array() ) {
        bsoncxx::builder::basic::array arr;
        for( auto &value: v )
            append(arr,value);
        x.append(arr);
    } else
    if( v.is_null() )
        x.append(bsoncxx::types::b_null());
    else
    if( v.is_boolean() )
        x.append(v.get<bool>());
    else
    if( v.is_number_integer() || v.is_number_unsigned() )
        x.append(v.get<int>());
    else
    if( v.is_number_float() ) {
        if( auto y = v.get<double>(); std::isnan(y) )
            x.append(bsoncxx::types::b_null());
        else
            x.append(y);
    }
    else
    if( v.is_string() ) {
        auto s = v.get<std::string>();
        date::year_month_day ymd;
        std::istringstream in {s};
        in >> date::parse("%F",ymd);
        if(in)
            x.append(bsoncxx::types::b_date(date::sys_days(ymd)));
        else
            x.append(v.get<std::string>());
    }
    else
        logger->error("append: unsupported type={}",static_cast<int>(v.type()));
}

void
append(
    bsoncxx::builder::basic::document &x,
    const std::string &k,
    const json &v
){
    if( v.is_object() ) {
        bsoncxx::builder::basic::document doc;
        for( auto &[key,value]: v.items() )
            append(doc,key,value);
        x.append(kvp(k,doc));
    } else
    if( v.is_array() ) {
        bsoncxx::builder::basic::array arr;
        for( auto &value: v )
            append(arr,value);
        x.append(kvp(k,arr));
    } else
    if( v.is_null() )
        x.append(kvp(k,bsoncxx::types::b_null()));
    else
    if( v.is_boolean() )
        x.append(kvp(k,v.get<bool>()));
    else
    if( v.is_number_integer() || v.is_number_unsigned() )
        x.append(kvp(k,v.get<int>()));
    else
    if( v.is_number_float() ) {
        if( auto y = v.get<double>(); std::isnan(y) )
            x.append(kvp(k,bsoncxx::types::b_null()));
        else
            x.append(kvp(k,y));
    }
    else
    if( v.is_string() ) {
        auto s = v.get<std::string>();
        date::year_month_day ymd;
        std::istringstream in {s};
        in >> date::parse("%F",ymd);
        if(in)
            x.append(kvp(k,bsoncxx::types::b_date(date::sys_days(ymd))));
        else
            x.append(kvp(k,v.get<std::string>()));
    }
    else
        logger->error("append: unsupported type={} for [{}]",static_cast<int>(v.type()),k);
}

bsoncxx::builder::basic::document
create_doc(
    const json &j
){
    bsoncxx::builder::basic::document doc {};
    for( auto &[key,value]: j.items() )
        append(doc,key,value);
    return doc;
}

void
save_json (
    mongocxx::collection col,
    const json &doc,
    const json &filter
){
    // logger->debug("save_json: {}   {}",filter.dump(),doc.dump());
    col.replace_one(
        create_doc(filter).view(),
        create_doc(doc).view(),
        mongocxx::options::replace().upsert(true)
    );
}

json DB::LoadJSON (
    const std::string &coll,
    const json &j,
    DBOptions opts
){
    std::lock_guard<std::recursive_mutex> lock(db_mutex);
    return load_json(collection(coll),j,opts&DBOptions::ONE);
}

void DB::SaveJSON (
    const std::string &coll,
    const json &doc,
    const json &filter
){
    std::lock_guard<std::recursive_mutex> lock(db_mutex);
    save_json(collection(coll),doc,filter);
}

json DB::Delete (
    const std::string &coll,
    const json &filter
) {
    auto
        c = collection(coll);
    c.delete_many( create_doc(filter).view() );
    return {};
}

std::string
replace_inf (
    std::string s
){
    static std::regex
        e (" ([+-])?(inf)([ ,])?");
    static std::string
        r = std::string(" $01") + "99e99" + std::string("$03");
    return
        std::regex_replace (s,e,r);
}

std::string
json_sanitize (
    std::string s
){
    s = replace_nan_to_null(s);
    s = replace_inf(s);
    return s;
}

std::string
replace_nan_to_null (
    std::string s
){
    for( size_t pos=0; true; ) {
        static const std::string_view
            nan_str(" nan,"),
            null_str(" null,");
        pos = s.find(nan_str,pos);
        if( pos==std::string::npos )
            break;
        s.replace(pos,nan_str.size(),null_str);
        pos += null_str.size();
    }
    return s;
}

json
load_json (
    mongocxx::collection col,
    const json &j,
    bool find_one
) try {
    logger->debug("load_json {}",j.dump());
    auto doc = create_doc(j);

    json rv;
    if(find_one) {
        auto x = col.find_one(doc.view());
        if(x)
            rv = json::parse(
                    json_sanitize(
                        bsoncxx::to_json(
                            x.value()
            )));
    } else {
        rv = nlohmann::json::array();
        mongocxx::cursor cursor = col.find(doc.view());

        for(auto &doc : cursor) {
            auto v1 = bsoncxx::to_json(doc);
            logger->debug("  convertion: {} => {}",j.dump(),v1);

            rv.push_back(
                json::parse(
                    json_sanitize(
                        v1
            )));
        }
    }

    return rv;
} catch ( const std::exception &e ) {
    logger->error(
        fmt::format(
            "load_json({}) => {}",
            j.dump(),
            e.what()
    ));
    throw;
}
