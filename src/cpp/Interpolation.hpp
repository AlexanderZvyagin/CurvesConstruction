#pragma once

#include <map>
#include <cmath>
#include <memory>
#include <functional>
#include <stdexcept>
#include <cstdio>
#include <span>

#include <gsl/gsl_spline.h>
#include <gsl/gsl_errno.h>

#include "gsl_init.hpp"

namespace math {

template <typename T>
class InterpolatorPiecewiseConstant {
protected:
    std::map<T,T> xy;
public:

    InterpolatorPiecewiseConstant(
        std::span<T> x,
        std::span<T> y
    ) {
        for(unsigned i=0; i<std::min(x.size(),y.size()); i++)
            xy[x[i]] = y[i];
    }

    T operator () (T x) const {
        if(xy.empty())
            throw std::runtime_error("InterpolatorPiecewiseConstant::operator(): empty interpolator");
        auto it = xy.upper_bound(x);
        if(it==xy.end()) return xy.rbegin()->second;
        it--;
        return it==xy.end() ? NAN: it->second;
    }

    T EvalOr (T x,T value_on_fail=NAN) const try {
        return (*this) (x);
    } catch (...) {
        return value_on_fail;
    }

    T Integral (T a,T b) const {
        auto it = xy.upper_bound(a);
        // printf("Integral from %g to %g.  First t=%g\n",a,b,it->first);
        T x {a}, integral {0};

        while(x<b){
            if(it==xy.end())
                throw std::logic_error(__PRETTY_FUNCTION__);
            // printf("x=[%g,%g] dx=%g y=%g",x,it->first,it->first-x,it->second);
            float dx = it->first - x;
            x = it->first;
            if(dx<0) break;
            integral += dx*it->second;
            it++;
            // printf(" %s\n",it==xy.end()?"end":"cont");
        }

        // printf("Integral = %g\n",integral);

        return integral;
    }

    std::map<T,T>       & GetXY (void)       {return xy;}
    std::map<T,T> const & GetXY (void) const {return xy;}
};

// https://www.gnu.org/software/gsl/doc/html/interp.html#c.gsl_interp_type
// gsl_interp_type *gsl_interp_linear
//
//     Linear interpolation.
//     This interpolation method does not require any additional memory.
//
// gsl_interp_type *gsl_interp_polynomial
//
//     Polynomial interpolation.
//     This method should only be used for interpolating small numbers
//     of points because polynomial interpolation introduces large
//     oscillations, even for well-behaved datasets.
//     The number of terms in the interpolating polynomial
//     is equal to the number of points.
//
// gsl_interp_type *gsl_interp_cspline
//
//     Cubic spline with natural boundary conditions.
//     The resulting curve is piecewise cubic on each interval,
//     with matching first and second derivatives at the supplied data-points.
//     The second derivative is chosen to be zero at the first point
//     and last point.
//
// gsl_interp_type *gsl_interp_cspline_periodic
//
//     Cubic spline with periodic boundary conditions.
//     The resulting curve is piecewise cubic on each interval, with matching
//     first and second derivatives at the supplied data-points.
//     The derivatives at the first and last points are also matched.
//     Note that the last point in the data must have the same y-value
//     as the first point, otherwise the resulting periodic interpolation will
//     have a discontinuity at the boundary.
//
// gsl_interp_type *gsl_interp_akima
//
//     Non-rounded Akima spline with natural boundary conditions.
//     This method uses the non-rounded corner algorithm of Wodicka.
//
// gsl_interp_type *gsl_interp_akima_periodic
//
//     Non-rounded Akima spline with periodic boundary conditions.
//     This method uses the non-rounded corner algorithm of Wodicka.
//
// gsl_interp_type *gsl_interp_steffen
//
//     Steffen’s method guarantees the monotonicity of the interpolating
//     function between the given data points.
//     Therefore, minima and maximgsl_interp_typea can only occur exactly at the data points,
//     and there can never be spurious oscillations between data points.
//     The interpolated function is piecewise cubic in each interval.
//     The resulting curve and its first derivative are guaranteed
//     to be continuous, but the second derivative may be discontinuous.

class Interpolator1D {
public:

    enum Type {None,PiecewiseConstant,Linear,Polynomial,CubicSpline,CubicSplinePeriodic,Akima,AkimaPeriodic,Steffen};

    static const gsl_interp_type *GetGSLType (Type itype) {
        switch(itype){
            case None:                  return nullptr;
            case PiecewiseConstant:     return nullptr;
            case Linear:                return gsl_interp_linear;
            case Polynomial:            return gsl_interp_polynomial;
            case CubicSpline:           return gsl_interp_cspline;
            case CubicSplinePeriodic:   return gsl_interp_cspline_periodic;
            case Akima:                 return gsl_interp_akima;
            case AkimaPeriodic:         return gsl_interp_akima_periodic;
            case Steffen:               return gsl_interp_steffen;
            default: throw std::invalid_argument(__PRETTY_FUNCTION__);
        }
    }

    using interpolation_t = std::unique_ptr<gsl_spline, std::function<void(gsl_spline*)> >;
    using acc_t = std::unique_ptr<gsl_interp_accel, std::function<void(gsl_interp_accel*)> >;

    Interpolator1D(void) : itype(None), type(nullptr) {}

    Interpolator1D(const Interpolator1D &f) : Interpolator1D() {*this = f;}

    Interpolator1D(
        std::span<double> x,
        std::span<double> y,
        Type itype
    )
        : itype(itype)
        , type (GetGSLType(itype))
    {
        if(type){
            const auto size = std::min (x.size(), y.size());
            interpolation = interpolation_t (
                gsl_spline_alloc(type, size),
                [] (gsl_spline *g) {gsl_spline_free(g);}
            );

            if( auto code=gsl_spline_init (interpolation.get(), x.data(), y.data(), size); code!=GSL_SUCCESS )
                throw std::runtime_error(std::string("Interpolator1D::Interpolator1D: gsl_spline_init ")+gsl_strerror(code));

            acc = acc_t( gsl_interp_accel_alloc (), [] (gsl_interp_accel*a) {gsl_interp_accel_free(a);} );
        }
        if(itype==PiecewiseConstant){
            iconst.reset(new InterpolatorPiecewiseConstant<double>(x,y));
        }
    }

    template<typename F>
    Interpolator1D(
        F f,
        const std::pair<double,double> &range,
        size_t intervals,
        Type itype=Type::CubicSpline
    )
        : itype(itype)
        , type (GetGSLType(itype))
    {
        if( intervals==0 )
            throw std::invalid_argument("Interpolator1D: 'intervals' must be positive");

        std::unique_ptr<double[]>
            x( new double[intervals+1]),
            y( new double[intervals+1]);

        x[0] = range.first;
        y[0] = f(x[0]);
        x[intervals] = range.second;
        y[intervals] = f(x[intervals]);

        const double dx = (range.second-range.first) / intervals;
        for( size_t i=1; i<intervals; i++ ) {
            x[i] = range.first + dx*i;
            y[i] = f(x[i]);
        }

        if(type){
            interpolation = interpolation_t (
                gsl_spline_alloc(type, intervals+1),
                [] (gsl_spline*g) {gsl_spline_free(g);}
            );

            if( auto code=gsl_spline_init (interpolation.get(), x.get(), y.get(), intervals+1); code!=GSL_SUCCESS )
                throw std::runtime_error(std::string("Interpolator1D::Interpolator1D: gsl_spline_init ")+gsl_strerror(code));

            acc = acc_t( gsl_interp_accel_alloc (), [] (gsl_interp_accel*a) {gsl_interp_accel_free(a);} );
        }
        if(itype==PiecewiseConstant){
            iconst.reset(new InterpolatorPiecewiseConstant<double>(
                std::span<double>(x.get(),intervals+1),
                std::span<double>(y.get(),intervals+1)
            ));
        }
    }

    double operator () (double x) const {
        if(itype==PiecewiseConstant)
            return (*iconst)(x);
        if(interpolation.get())
            return gsl_spline_eval(interpolation.get(),x,acc.get());
        throw std::invalid_argument("Interpolator1D was not initialized");
    }

    double EvalOr (double x,double value_on_fail=NAN) const try {
        return (*this)(x);
    } catch (...) {
        return value_on_fail;
    }

    Interpolator1D(Interpolator1D &&f) {
        interpolation = std::move(f.interpolation);
        iconst = std::move(f.iconst);
        acc = std::move(f.acc);
        type = f.type;
        itype = f.itype;
    }

    Interpolator1D & operator = (const Interpolator1D &f) {
        type = f.type;
        itype = f.itype;
        acc.reset(nullptr);
        interpolation.reset(nullptr);
        iconst.reset(static_cast<InterpolatorPiecewiseConstant<double>*>(nullptr));

        if(f.interpolation) {
            interpolation = interpolation_t (
                gsl_spline_alloc(type,f.interpolation->size),
                [] (gsl_spline*g) {gsl_spline_free(g);}
            );

            if( auto code=gsl_spline_init (interpolation.get(), f.interpolation->x, f.interpolation->y, f.interpolation->size); code!=GSL_SUCCESS )
                throw std::runtime_error(gsl_strerror(code));

            acc = acc_t( gsl_interp_accel_alloc (), [] (gsl_interp_accel*a) {gsl_interp_accel_free(a);} );
        }
        if(itype==PiecewiseConstant){
            iconst.reset(new InterpolatorPiecewiseConstant<double>(*f.iconst));
        }
        return *this;
    }

    Interpolator1D & operator = (Interpolator1D &&f) {
        if( &f!=this ) {
            interpolation = std::move(f.interpolation);
            iconst = std::move(f.iconst);
            acc = std::move(f.acc);
            type = f.type;
            itype = f.itype;
        }
        return *this;
    }

    // unsigned Size (void) const {
    //     if(itype==PiecewiseConstant)
    //         return iconst->xy.size();
    //     return interpolation ? interpolation.get()->size : 0;
    // }
    //
    // std::pair<double,double> operator [] (unsigned i) const {
    //     const auto s = interpolation.get();
    //     if(s and i<s->size)
    //         return {s->x[i],s->y[i]};
    //     throw std::invalid_argument("Interpolator1D::operator[]");
    // }

    std::string Name (void) const {
        switch(itype){
            case None: return "None";
            case PiecewiseConstant: return "PiecewiseConstant";
            default: return interpolation ? gsl_spline_name(interpolation.get()) : "None";
        }
    }

    std::string GetTypeName (void) const {
        switch(itype){
            case None:                  return "NONE";
            case PiecewiseConstant:     return "PiecewiseConstant";
            case Linear:                return "Linear";
            case Polynomial:            return "Polynomial";
            case CubicSpline:           return "CubicSpline";
            case CubicSplinePeriodic:   return "CubicSplinePeriodic";
            case Akima:                 return "Akima";
            case AkimaPeriodic:         return "AkimaPeriodic";
            case Steffen:               return "Steffen";
            default: throw std::invalid_argument(__PRETTY_FUNCTION__);
        }
    }

    double Integral (double a,double b) const {
        if(itype==Type::PiecewiseConstant)
            return iconst->Integral(a,b);
        if(!interpolation)
            throw std::invalid_argument("Interpolator1D::Integral(): empty curve");
        double result;
        auto status = gsl_spline_eval_integ_e(interpolation.get(),a,b,acc.get(),&result);
        if(status!=GSL_SUCCESS)
            throw std::runtime_error(gsl_strerror(status));
        return result;
    }

    int GetSize (void) const {
        if(interpolation)
            return interpolation->size;
        if(iconst)
            return iconst->GetXY().size();
        return 0;
    }

    std::span<double> GetX (void) const {
        if(interpolation)
            return {interpolation->x,interpolation->size};
        else
            throw std::invalid_argument(__PRETTY_FUNCTION__);
    }
    std::span<double> GetY (void) const {
        if(interpolation)
            return {interpolation->y,interpolation->size};
        else
            throw std::invalid_argument(__PRETTY_FUNCTION__);
    }

    void Print (void) const {
        // if(interpolation){
        //     printf("(x,y) %u points: ",Size());
        //     for(unsigned i=0;i<Size();i++){
        //         printf(" (%g,%g)",GetX()[i],GetY()[i]);
        //     }
        //     printf("\n");
        // }
    }

    Type GetType (void) const {return itype;}

protected:

    Type itype;
    const gsl_interp_type *type;
    interpolation_t interpolation;
    acc_t acc;
    std::shared_ptr<InterpolatorPiecewiseConstant<double>> iconst;

    friend std::ostream & operator << (std::ostream &os, const Interpolator1D &v);
};

inline
std::ostream &
operator << (std::ostream &os, const Interpolator1D &v) {
    const auto s = v.interpolation.get();
    if(s) {
        os << "(Interpolator1D " << s->size << " points "
           << "x=[" << s->x [0] << "," << s->x [s->size-1] << "]) "
           << "y=[" << s->y [0] << "," << s->y [s->size-1] << "])";
    } else {
        os << "(Interpolator1D uninitialized)";
    }
    return os;
}

}
