#pragma once

#define ONLY_C_LOCALE 0

#include <utility>
#include <string>
#include <date/date.h>

// https://stackoverflow.com/questions/5966594/how-can-i-use-pragma-message-so-that-the-message-points-to-the-filelineno
// #pragma message(Reminder "Fix this problem!")
#define Stringize( L )     #L
#define MakeString( M, L ) M(L)
#define $Line MakeString( Stringize, __LINE__ )
#define Reminder __FILE__ "(" $Line ") : Reminder: "

inline bool
starts_with (
    std::string_view s1,
    std::string_view s2
){
    return 0==s1.compare(0,s2.size(),s2);
}

std::pair<unsigned,unsigned>
decode_FRA_periods (
    std::string_view name
);

std::string to_str(const date::year_month_day &d);
