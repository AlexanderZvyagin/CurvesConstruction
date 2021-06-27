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

    using math::Interpolator1D::operator =; // (math::Interpolator1D &&);

    YieldCurve (const gsl_interp_type *_type=gsl_interp_linear) {
        type = _type;
    }
    YieldCurve (std::span<double> vx, std::span<double> vy, const gsl_interp_type *type=gsl_interp_linear)
        : math::Interpolator1D(vx,vy,type)
    {}

    const std::map <float,std::shared_ptr<Instrument>> &
        GetInstruments (void) const {return instruments;}

    YieldCurve & Add (const Instrument &x);

    YieldCurve & Build (const gsl_interp_type *type=gsl_interp_linear,float yield_to_infinity=0);

    float GetForwardRate (float t1,float t2) const {
        return Integral(t1,t2);
    }

    float GetDiscountFactor (float t1,float t2) const {
        return std::exp(-Integral(t1,t2));
    }

    float GetDiscountFactor (float t1) const {
        return std::exp(-Integral(0,t1));
    }

    void Print(void) const;
};
