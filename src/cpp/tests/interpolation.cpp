#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cmath>
#include <cstdio>

#include "Interpolation.hpp"

using namespace math;

TEST_CASE("basic"){
    Interpolator1D f;
    CHECK_THROWS_AS( f(1), std::exception);
}

TEST_CASE("compare"){
    auto
        f = [] (double x) {return x*x;};
    const std::pair<double,double>
        range {-5,5};
    const int
        intervals {2};


    Interpolator1D
        g1(f,range,intervals,Interpolator1D::Type::Linear),
        g2(f,range,intervals,Interpolator1D::Type::CubicSpline);
    printf("Interpolation names: g1=%s g2=%s\n",g1.Name().c_str(),g2.Name().c_str());
    for(float x=range.first;x<=range.second;x += (range.second-range.first)/10)
        printf("x=%5g:  f(x)=%10g  g1(x)=%10g  g2(x)=%10g\n",x,f(x),g1(x),g2(x));
    printf("integral: %g\n",g1.Integral(1,2));
}

void check_interpolation(Interpolator1D::Type type) try {
    auto
        f = [] (double x) {return x*x;};
    std::vector<double> vx {1,2,3,4}, vy;
    for(auto x: vx) vy.push_back(f(x));

    Interpolator1D
        g1(std::span<double>{vx.data(),vx.size()-1},std::span<double>{vy.data(),vy.size()-1},type),
        g2(vx,vy,type);
    printf("Interpolation type: %s\n",g1.Name().c_str());

    g1.Print();
    g2.Print();

    for(float x=vx.front(), dx=(vx.back()-vx.front())/10; x<=vx.back(); x+=dx){
        printf("x=%10g f(x)=%10g g1(x)=%10g g2(x)=%10g\n",x,f(x),g1.EvalOr(x),g2.EvalOr(x));
    }
} catch (const std::exception &e) {
    printf("Error: %s\n",e.what());
}

TEST_CASE("cubic"){
    auto
        f = [] (double x) {return x*x;};
    std::vector<double> vx {1,2,3,4}, vy;
    for(auto x: vx) vy.push_back(f(x));

    Interpolator1D
        g1(std::span<double>{vx.data(),vx.size()-1},std::span<double>{vy.data(),vy.size()-1},Interpolator1D::Type::CubicSpline),
        g2(vx,vy,Interpolator1D::Type::CubicSpline);

    g1.Print();
    g2.Print();

    for(float x=vx.front(), dx=0.5; x<=5; x+=dx){
        printf("x=%10g f(x)=%10g g1(x)=%10g g2(x)=%10g\n",x,f(x),g1.EvalOr(x),g2.EvalOr(x));
    }
}

TEST_CASE("compare-all"){
    check_interpolation(Interpolator1D::Type::PiecewiseConstant);
    check_interpolation(Interpolator1D::Type::Linear);
    check_interpolation(Interpolator1D::Type::Polynomial);
    check_interpolation(Interpolator1D::Type::CubicSpline);
    check_interpolation(Interpolator1D::Type::CubicSplinePeriodic);
    check_interpolation(Interpolator1D::Type::Akima);
    check_interpolation(Interpolator1D::Type::AkimaPeriodic);
    check_interpolation(Interpolator1D::Type::Steffen);
}

TEST_CASE("yconst"){
    std::vector<double> vx {1,2,5}, vy {0,0,0};

    Interpolator1D
        g(vx,vy,Interpolator1D::Type::Linear);

    g.Print();

    for(float x=vx.front(), dx=(vx.back()-vx.front())/10; x<=vx.back(); x+=dx){
        printf("x=%10g g(x)=%10g\n",x,g(x));
    }
}

TEST_CASE("const"){
    std::vector<float> vx {1,2}, vy {1,4};

    InterpolatorPiecewiseConstant<float>
        g(vx,vy);

    // g.Print();

    for(float x=-1; x<=10; x+=0.5){
        printf("x=%10g g(x)=%10g\n",x,g(x));
    }

    g.Integral(-100,2);
    g.Integral(0.5,2);
    g.Integral(1.5,2);
    g.Integral(1.9,2);

    CHECK_THROWS_AS( g.Integral(1.9,2.1), std::exception);
}
