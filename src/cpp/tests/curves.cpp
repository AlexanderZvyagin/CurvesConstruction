#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstdio>

#include "YieldCurve.hpp"
#include "Instrument.hpp"
#include "ZeroCouponBond.hpp"
#include "ForwardRateAgreement.hpp"
#include "Swap.hpp"
#include "common.hpp"

TEST_CASE("zcb"){
    YieldCurve curve;

    curve
        .Add (ZeroCouponBond(2,0.8))
        .Add (ZeroCouponBond(1,0.9))
        .Add (ZeroCouponBond(5,0.6))
        .Build ()
    ;
    curve.Print();
}

TEST_CASE("fra"){
    YieldCurve curve;

    curve
        .Add (ForwardRateAgreement(0,1,0.01))
        .Add (ForwardRateAgreement(0.5,2,0.02))
        .Add (ForwardRateAgreement(0,3,0.03))
        .Build ()
    ;
    curve.Print();
}

TEST_CASE("swap"){

    YieldCurve
        curve;

    Swap swap1;
    if(1) {
        // Finalyze the swap

        LegFixed &lfix = swap1.lfix;
        lfix.t0 = 0;
        lfix.dt = 1;
        lfix.n = 10;
        lfix.rate = 0.01;

        LegFloat &lflt = swap1.lflt;
        lflt.t0 = lfix.t0;
        lflt.dt = lfix.dt;
        lflt.n  = lfix.n;
        lflt.curve = &curve;
    }

    curve
        .Add (swap1)
        .Build (math::Interpolator1D::Type::Linear)
    ;
    curve.Print();
}

TEST_CASE("build1"){
    YieldCurve curve;

    curve
        .Add (ForwardRateAgreement(0,1*month,-0.56200*pc))
        .Add (ForwardRateAgreement(0,3*month,-0.53800*pc))
        .Add (ForwardRateAgreement(0,6*month,-0.51300*pc))
        .Add (ForwardRateAgreement(0,1*year ,-0.47800*pc))
        .Build (math::Interpolator1D::Type::Linear)
    ;
    curve.Print();
}
