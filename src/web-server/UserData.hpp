#pragma once

#include <optional>
#include <nlohmann/json.hpp>
// #include "fmt/ostream.h"

struct UserData {
    using json = nlohmann::json;
    UserData (void);
    ~UserData (void);
    json Call(const json &) noexcept;

    void build_curve    (const json &data, json &pld);
    void get_quotes     (const json &data, json &pld);
};

// nlohmann::json UserData_Request1 (const nlohmann::json &payload);

// inline
// std::vector<double> linear_space(unsigned points,double min,double max) {
//     std::vector<double> vx;
//     switch(points) {
//         case 0:
//             return {};
//         case 1:
//             if( min!=max )
//                 throw std::invalid_argument("linear_space, min!=max for points=1");
//             return {min};
//         default: {
//             double const dx = (max-min)/(points-1);
//             vx.push_back(min);
//             for( size_t i=1; i<points-1; i++ )
//                 vx.push_back(min + dx*i);
//             vx.push_back(max);
//             return vx;
//         }
//     }
// }
//
// template<typename T>
// T value_or(const nlohmann::json &x,const std::string &name,std::optional<T> defval={}) {
//     auto it = x.find(name);
//     if( it!=x.end() )
//         return it->get<T>();
//     else
//         if( defval.has_value() )
//             return defval.value();
//         else
//             throw std::runtime_error(fmt::format("unknown key '{}'",name));
// }
//
// template<typename T>
// T arg(const nlohmann::json &x,const std::string &name,std::optional<T> defval={}) {
//     return value_or(x.at("args"),name,defval);
// }
//
// struct PlotsArg {
//     size_t points;
//     double min,max;
//     // std::vector<std::string> plots;
// };
//
// inline
// void from_json(const nlohmann::json& j, PlotsArg& p) {
//     p.points = value_or<size_t>(j,"points");
//     p.min = value_or<double>(j,"min");
//     p.max = value_or<double>(j,"max");
// }
//
// inline
// std::vector<double> build_vx_points(const PlotsArg &p) {
//     std::vector<double> vx;
//     if( p.points<2 or p.min>=p.max )
//         throw std::invalid_argument("build_vx_points: bad args");
//     double const dx = (p.max-p.min)/(p.points-1);
//     for( size_t i=0; i<p.points; i++ )
//         vx.push_back(p.min + dx*i);
//     return vx;
// }
//
// template<typename F>
// std::vector<double> calc_vy(F func,const std::vector<double> &vx) {
//     std::vector<double> vy;
//     for( auto x: vx ) {
//         double value;
//         try {
//             value = func(x);
//         } catch (...) {
//             value = NAN;
//         }
//         vy.push_back(value);
//     }
//     return vy;
// }
