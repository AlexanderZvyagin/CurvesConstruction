#include <vector>
#include <stdexcept>
#include <fmt/ostream.h>

#include "utils.hpp"
//#include "logs.hh"

// months
std::pair<unsigned,unsigned>
decode_FRA_periods (
    std::string_view name
){
    // 'EUFR0J1J' # FRA 10M x 22M '''
    if( name=="EUR006M" )
        return {0,6};

    static const std::string fra_prefix = "EUFR";

    if( not starts_with(name,fra_prefix) )
        throw std::invalid_argument(fmt::format("decode_FRA_periods: instrument '{}' does not start from '{}'",name,fra_prefix));

    std::vector<unsigned> periods_month;

    auto decode_months = [] (std::string_view s) {
        size_t
            i {0}, // characters counter
            l {0}; // months

        if( i<s.size() and s[i]=='0')
            i ++;

        if( i<s.size() and s[i]>='1' and s[i]<='9' ) {
            l += 12 * (s[i] - '0');
            i++;
        }

        if( i<s.size() and s[i]>='A' and s[i]<='K' ) {
            l += s[i] - 'A' + 1;
            i++;
        }

        return std::make_pair(l,s.substr(i,s.size()-i));
    };

    auto [l1,s1] = decode_months(name.substr(fra_prefix.size(),name.size()-fra_prefix.size()));
    auto [l2,s2] = decode_months(s1);
    // fmt::print("'{}' => '{}' => '{}'\n",name,s1,s2);

    if( not s2.empty() )
        throw std::invalid_argument(fmt::format("decode_FRA_periods: bad name '{}' => '{}' => '{}'",name,s1,s2));

    return {l1,l2};
}

std::string to_str(const date::year_month_day &d) {
    return fmt::format(
        "{:4d}-{:02d}-{:02d}",
        static_cast<int>(d.year()),
        static_cast<unsigned>(d.month()),
        static_cast<unsigned>(d.day())
    );
}
