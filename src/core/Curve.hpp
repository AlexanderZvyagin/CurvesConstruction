#pragma once

#include <stdexcept>
#include <memory>
#include <map>
#include <cmath>

#include "Interpolation.hpp"

class Instrument;

class Curve: public math::Interpolator1D {
private:
    std::map <float,std::shared_ptr<Instrument>> instruments;
public:

    using math::Interpolator1D::operator =;

    Curve (
        void
    ) {}

    Curve (
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

    Curve & Add (const Instrument &x);

    math::Result
    Build (
        math::Interpolator1D::Type t = math::Interpolator1D::Type::CubicSpline
    );

    math::Result
    Build (
        math::Interpolator1D::Type t,
        const math::Options &opts
    );

    math::Result
    BuildPiecewiseConstant (void);

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

    Curve & SetYield (float t,float yield) {
        if(iconst)
            iconst -> GetXY() [t] = yield;
        else
            throw std::invalid_argument("Curve::SetYield() is only available for PiecewiseConstant yield curve.");
        return *this;
    }

    void Print (void) const;
};
