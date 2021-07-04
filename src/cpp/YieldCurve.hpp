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
        const std::vector<double> &vx,
        const std::vector<double> &vy,
        math::Interpolator1D::Type itype
    )
        : math::Interpolator1D (vx,vy,itype)
    {}

    const std::map <float,std::shared_ptr<Instrument>> &
    GetInstruments (
        void
    ) const {return instruments;}

    YieldCurve & Add (const Instrument &x);

    YieldCurve &
    Build (
        math::Interpolator1D::Type t = math::Interpolator1D::Type::CubicSpline,
        float yield_to_infinity = 0
    );

    YieldCurve & BuildPiecewiseConstant (void);

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

    float
    GetMaturity (
        void
    ) const {
        if(interpolation && interpolation->size>0)
            return interpolation->x[interpolation->size-1];
        if(iconst){
            auto it=iconst->GetXY().rbegin();
            if(it!=iconst->GetXY().rend())
                return it->first;
        }
        throw std::invalid_argument(__PRETTY_FUNCTION__);
    }

    YieldCurve & SetYield (float t,float yield) {
        if(iconst)
            iconst -> GetXY() [t] = yield;
        else
            throw std::invalid_argument("YieldCurve::SetYield() is only available for PiecewiseConstant yield curve.");
        return *this;
    }

    void Print (void) const;
};
