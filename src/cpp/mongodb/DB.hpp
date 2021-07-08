#pragma once

#include <mutex>

#include <nlohmann/json.hpp>
#include <mongocxx/client.hpp>
#include <bsoncxx/json.hpp>

#include "utils.hpp"

using nlohmann::json;

extern std::recursive_mutex db_mutex;

// collection of options related to DB operations
enum DBOptions {
    NONE        = 0,
    REPLACE     = 1,
    ONE         = 1<<1,
    MANY        = 1<<2
};

bsoncxx::builder::basic::document
create_doc(
    const nlohmann::json &j
);

std::string
replace_nan_to_null (
    std::string s
);

template<typename T>
void save (
    mongocxx::collection coll,
    const nlohmann::json &query,
    T const &obj,
    DBOptions opts
){
    nlohmann::json doc = query;
    doc["payload"] = obj;

    if( opts&DBOptions::REPLACE )
        coll.replace_one(
            create_doc(query).view(),
            create_doc(doc).view(),
            mongocxx::options::replace().upsert(true)
        );
    else
        throw std::invalid_argument("save(DB): unsupported 'opts' argument");
}

template<typename T>
std::optional<T> load(
    mongocxx::collection coll,
    const nlohmann::json &query,
    DBOptions opts
){

    if( opts&DBOptions::ONE ) {
        auto doc = create_doc(query);
        auto x = coll.find_one(doc.view());

        std::optional<T> rv;
        if(x)
            rv = nlohmann::json::parse(
                    replace_nan_to_null(
                        bsoncxx::to_json(
                            x.value()
            ))).at("payload").get<T>();
        return rv;
    } else
        throw std::invalid_argument("load(DB): unsupported 'opts' argument");
}

template<typename T>
std::vector<T> load_many(
    mongocxx::collection coll,
    const nlohmann::json &query
){
    std::vector<T> rv;
    auto v = coll.find(create_doc(query).view());
    for( auto &x : v) {
        auto
            y = nlohmann::json::parse(
                replace_nan_to_null(
                    bsoncxx::to_json(
                        x
        ))).at("payload").get<T>();

        rv.emplace_back(y);
    }
    return rv;
}

// class Swaption;

using MarketData = std::unordered_map<std::string,double>;

class DB {
public:

    DB(const std::string &db_server,const std::string &db_name, const std::string &quotes_collection);

    template<typename T> void Save (
        const std::string &coll,
        const nlohmann::json &query,
        T const &obj,
        DBOptions opts
    ){
        std::lock_guard<std::recursive_mutex> lock(db_mutex);
        save( collection(coll), query, obj, opts );
    }

    template<typename T> std::optional<T> Load (
        const std::string &coll,
        const nlohmann::json &query,
        DBOptions opts
    ){
        std::lock_guard<std::recursive_mutex> lock(db_mutex);
        return load<T>( collection(coll), query, opts );
    }

    template<typename T> std::vector<T> LoadMany (
        const std::string &coll,
        const nlohmann::json &query
    ){
        std::lock_guard<std::recursive_mutex> lock(db_mutex);
        return load_many<T>( collection(coll), query );
    }

    json LoadJSON (
        const std::string &coll,
        const json &j,
        DBOptions opts
    );

    json Delete (
        const std::string &coll,
        const json &j
    );

    void SaveJSON (
        const std::string &coll,
        const json &doc,
        const json &filter
    );

    MarketData LoadData(date::year_month_day d);

    // std::vector<Swaption> LoadSwaptionsStructure(const std::string &swaptions_index );
    //
    // void LoadSwaptions (date::year_month_day d, std::vector<Swaption> &swaptions );
    //
    // std::optional<Curves> LoadCurves (
    //     date::year_month_day ymd,
    //     const std::vector<std::string> &tags
    // );
    //
    // void SaveCurves (
    //     const Curves &curves,
    //     date::year_month_day ymd,
    //     const std::vector<std::string> &tags
    // );

private:

    std::string db_server, db_name, quotes_collection;
    mongocxx::client conn;
    mongocxx::collection collection(const std::string &name) {
        return conn [db_name] [name];
    }
};
