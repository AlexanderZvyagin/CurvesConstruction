#pragma once

#include <stdexcept>
#include <memory>
#include <map>
#include <cmath>

#include "Interpolation.hpp"

class Instrument;

class YieldCurve: public math::Interpolator1D {
private:
    std::map <float,std::shared_ptr<Instrument>> instruments;
public:

    using math::Interpolator1D::operator =;

    YieldCurve (
        void
    ) {}

    YieldCurve (
        std::span<double> vx,
        std::span<double> vy,
        math::Interpolator1D::Type itype
    )
        : math::Interpolator1D (vx,vy,itype)
    {}

    const std::map <float,std::shared_ptr<Instrument>> &
    GetInstruments (
        void
    ) const {return instruments;}

    YieldCurve &
    Add (
        const Instrument &x
    );

    YieldCurve &
    Build (
        math::Interpolator1D::Type t = math::Interpolator1D::Type::CubicSpline,
        float yield_to_infinity = 0
    );

    float GetForwardRate (
        float t1,
        float t2
    ) const {
        return Integral(t1,t2)/(t2-t1);
    }

    float
    GetDiscountFactor (
        float t1,
        float t2
    ) const {
        return std::exp(-Integral(t1,t2));
    }

    float
    GetDiscountFactor (
        float t1
    ) const {
        return std::exp(-Integral(0,t1));
    }

    void
    Print (
        void
    ) const;
};
