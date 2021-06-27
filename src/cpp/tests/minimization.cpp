#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cmath>
#include <cstdio>
#include <span>

#include "minimizer.hpp"

using namespace math;

TEST_CASE("basic"){

    std::vector<Parameter> pars {Parameter(0,0.1,-10,10)};

    auto func = [] (const std::span<double> pars) {
        if(pars.size()!=1) throw std::invalid_argument("bad args");
        return std::pow(pars[0]-2.331,2);
    };

    auto r = math::minimize(
        func,
        pars,
        math::Options()
    );

    std::cout << r << "\n";
}

TEST_CASE("fit2"){

    std::vector<Parameter> pars {
        Parameter(0,0.1,-10,10),
        Parameter(0,0.1,-10,10)
    };

    auto func = [] (const std::span<double> pars) {
        if(pars.size()!=2) throw std::invalid_argument("bad args");
        return std::pow(pars[0]+1.331,2) + std::pow(pars[1]-2.331,4);
    };

    auto r = math::minimize(
        func,
        pars,
        math::Options()
    );

    std::cout << r << "\n";
}
