#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cassert>
#include <cmath>

#include <fmt/core.h>
#include <fmt/ostream.h>

#include "Swap.hpp"
#include "Curve.hpp"

TEST_CASE("SwapLegs"){

    Curve curve;
    // curve.SetYield(0,0.05);

    LegFixed lfix;
    lfix.t0 = 0;
    lfix.dt = 1;
    lfix.n = 2;
    lfix.rate = 0.05;

    LegFloat lflt;
    lflt.t0 = lfix.t0;
    lflt.dt = lfix.dt;
    lflt.n = lfix.n;
    lflt.curve = &curve;

    fmt::print("v_lfix = {}  PV={}\n",lfix.About(),lfix.Eval(curve));
    fmt::print("v_lflt = {}  PV={}\n",lflt.About(),lflt.Eval(curve));
}

// TEST_CASE("SwapLegs"){
//
//     Curve curve;
//     Curve curve_libor;
//     curve_libor.SetYield(TimePeriod(0),0.03);
//     curve.SetYield(TimePeriod(0),0.05);
//
//     LegFixed lfix;
//     lfix.t0 = TimePoint(0);
//     lfix.dt = TimePeriod(1);
//     lfix.n = 2;
//     lfix.rate = 0.05;
//
//     LegFloat lflt;
//     lflt.t0 = TimePoint(0);
//     lflt.dt = TimePeriod(1);
//     lflt.n = 2;
//     lflt.curve = &curve_libor;
//
//     fmt::print("v_lfix = {}  PV={}\n",lfix.About(),lfix.Eval(curve));
//     fmt::print("v_lflt = {}  PV={}\n",lflt.About(),lflt.Eval(curve));
// }
//
// TEST_CASE("SwapLegs"){
//
//     Curve curve ;
//     Curve curve_libor;
//
//     curve_libor.SetYield(TimePeriod(0),0.03);
//     curve.SetYield(TimePeriod(0),0.05);
//
//     Swap swap;
//
//     LegFixed &lfix = swap.lfix;
//     lfix.t0 = TimePoint(0);
//     lfix.dt = TimePeriod(1);
//     lfix.n = 2;
//     lfix.rate = 0.05;
//
//     LegFloat &lflt = swap.lflt;
//     lflt.t0 = TimePoint(0);
//     lflt.dt = TimePeriod(1);
//     lflt.n = 2;
//     lflt.curve = &curve_libor;
//
//     fmt::print("swap = {}  PV={}\n",swap.About(),swap.Eval(curve));
//
//     // curve_libor->yields[TimePeriod(0)] = 0.03;
//     // curve->yields[TimePeriod(0)] = 0.05;
//     //
//     // LegFixed lfix;
//     // lfix.t0 = TimePoint(0);
//     // lfix.dt = TimePeriod(1);
//     // lfix.n = 2;
//     // lfix.fixed_rate = 0.05;
//     //
//     // LegFloat lflt;
//     // lflt.t0 = TimePoint(0);
//     // lflt.dt = TimePeriod(1);
//     // lflt.n = 2;
//     // lflt.float_curve = curve_libor;
//     //
//     // fmt::print("v_lfix = {}  PV={}\n",lfix.About(),lfix.Eval(*curve));
//     // fmt::print("v_lflt = {}  PV={}\n",lflt.About(),lflt.Eval(*curve));
// }
