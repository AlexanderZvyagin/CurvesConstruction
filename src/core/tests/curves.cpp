#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstdio>

#include "YieldCurve.hpp"
#include "Instrument.hpp"
#include "ZeroCouponBond.hpp"
#include "ForwardRateAgreement.hpp"
#include "Swap.hpp"
#include "common.hpp"

void add_zcb(YieldCurve &curve){
    curve
        .Add (ZeroCouponBond(2,0.8))
        .Add (ZeroCouponBond(1,0.9))
        .Add (ZeroCouponBond(5,0.6))
    ;
}

TEST_CASE("zcb-PiecewiseConstant"){
    YieldCurve curve;

    add_zcb(curve);
    curve.Build (math::Interpolator1D::Type::PiecewiseConstant);
    curve.Print();
}

TEST_CASE("zcb"){
    YieldCurve curve;

    add_zcb(curve);
    curve.Build();
    curve.Print();
}

void add_fra(YieldCurve &curve){
    curve
        .Add (ForwardRateAgreement(0,1,0.01))
        .Add (ForwardRateAgreement(0.5,2,0.02))
        .Add (ForwardRateAgreement(0,3,0.03))
    ;
}

TEST_CASE("fra-PiecewiseConstant"){
    YieldCurve curve;

    add_fra(curve);
    curve.Build (math::Interpolator1D::Type::PiecewiseConstant);
    curve.Print();
}

TEST_CASE("fra"){
    YieldCurve curve;

    add_fra(curve);
    curve.Build ();
    curve.Print();
}

TEST_CASE("fra-pw2"){
    YieldCurve curve;
    curve
        .Add(ForwardRateAgreement(1,1,0.001))
        .Add(ForwardRateAgreement(2,1,0.0025))
        .Build (math::Interpolator1D::Type::PiecewiseConstant);
    curve.Print();

    // printf("GetForwardRate: %g\n",curve.GetForwardRate(1,))
}

Swap create_swap(
    float fair_rate,
    YieldCurve &float_curve,
    float maturity,
    float dt=0.5,
    float t0=0
){
    Swap swap;

    int n = std::round(maturity/dt);

    LegFixed &lfix = swap.lfix;
    lfix.t0 = t0;
    lfix.dt = dt;
    lfix.n = n;
    lfix.rate = fair_rate;

    LegFloat &lflt = swap.lflt;
    lflt.t0 = t0;
    lflt.dt = dt;
    lflt.n  = n;
    lflt.curve = &float_curve;

    return swap;
}

void add_swap(YieldCurve &curve){

    curve.Add(create_swap(0.01,curve,1*year));
    curve.Add(create_swap(0.02,curve,2*year));
    curve.Add(create_swap(0.03,curve,3*year));
    curve.Add(create_swap(0.05,curve,10*year));

    // Swap swap;
    //
    // if(1) {
    //     LegFixed &lfix = swap.lfix;
    //     lfix.t0 = 0;
    //     lfix.dt = 1;
    //     lfix.n = 10;
    //     lfix.rate = 0.01;
    //
    //     LegFloat &lflt = swap.lflt;
    //     lflt.t0 = lfix.t0;
    //     lflt.dt = lfix.dt;
    //     lflt.n  = lfix.n;
    //     lflt.curve = &curve;
    // }
    //
    // curve.Add(swap1);
}

TEST_CASE("swap-PiecewiseConstant"){
    YieldCurve curve;
    add_swap(curve);
    curve.Build (math::Interpolator1D::Type::PiecewiseConstant);
    curve.Print();
}

TEST_CASE("swap"){
    YieldCurve curve;
    add_swap(curve);
    curve.Build (math::Interpolator1D::Type::Linear);
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
