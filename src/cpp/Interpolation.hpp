#pragma once

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
//     Therefore, minima and maxima can only occur exactly at the data points,
//     and there can never be spurious oscillations between data points.
//     The interpolated function is piecewise cubic in each interval.
//     The resulting curve and its first derivative are guaranteed
//     to be continuous, but the second derivative may be discontinuous.


class Interpolator1D {
public:
    using interpolation_t = std::unique_ptr<gsl_spline, std::function<void(gsl_spline*)> >;
    using acc_t = std::unique_ptr<gsl_interp_accel, std::function<void(gsl_interp_accel*)> >;

    Interpolator1D(void) {}

    Interpolator1D(const Interpolator1D &f) : Interpolator1D() {
        *this = f;
    }

    Interpolator1D(std::span<double> x,std::span<double> y,const gsl_interp_type *type=gsl_interp_cspline )
        : type(type)
    {
        const auto size = std::min (x.size(), y.size());
        interpolation = interpolation_t (
            gsl_spline_alloc(type, size),
            [] (gsl_spline *g) {gsl_spline_free(g);}
        );

        if( auto code=gsl_spline_init (interpolation.get(), x.data(), y.data(), size); code!=GSL_SUCCESS )
            throw std::runtime_error(std::string("Interpolator1D::Interpolator1D: gsl_spline_init ")+gsl_strerror(code));

        acc = acc_t( gsl_interp_accel_alloc (), [] (gsl_interp_accel*a) {gsl_interp_accel_free(a);} );
    }

    // Interpolator1D(const std::vector<double> &x,const std::vector<double> &y,const gsl_interp_type *type=gsl_interp_cspline )
    //     : Interpolator1D( std::span{x.data(),x.size()}, std::span{y.data(),y.size()}, type)
    // {}

    // Interpolator1D(const std::vector<double> &x,const std::vector<double> &y,const gsl_interp_type *type=gsl_interp_cspline )
    //     : type(type)
    // {
    //     const auto size = std::min (x.size(), y.size());
    //     interpolation = interpolation_t (
    //         gsl_spline_alloc(type, size),
    //         [] (gsl_spline *g) {gsl_spline_free(g);}
    //     );
    //
    //     if( auto code=gsl_spline_init (interpolation.get(), x.data(), y.data(), size); code!=GSL_SUCCESS )
    //         throw std::runtime_error(std::string("Interpolator1D::Interpolator1D: gsl_spline_init ")+gsl_strerror(code));
    //
    //     acc = acc_t( gsl_interp_accel_alloc (), [] (gsl_interp_accel*a) {gsl_interp_accel_free(a);} );
    // }

    template<typename F>
    Interpolator1D(F f,const std::pair<double,double> &range,size_t intervals,const gsl_interp_type *type=gsl_interp_cspline)
        : type(type)
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

        interpolation = interpolation_t (
            gsl_spline_alloc(type, intervals+1),
            [] (gsl_spline*g) {gsl_spline_free(g);}
        );

        if( auto code=gsl_spline_init (interpolation.get(), x.get(), y.get(), intervals+1); code!=GSL_SUCCESS )
            throw std::runtime_error(std::string("Interpolator1D::Interpolator1D: gsl_spline_init ")+gsl_strerror(code));

        acc = acc_t( gsl_interp_accel_alloc (), [] (gsl_interp_accel*a) {gsl_interp_accel_free(a);} );
    }

    double operator () (double x) const {
        if(!interpolation.get())
            throw std::invalid_argument("Interpolator1D was not initialized");
        return gsl_spline_eval(interpolation.get(),x,acc.get());
    }

    double EvalOr (double x,double value_on_fail=NAN) const try {
        return (*this)(x);
    } catch (...) {
        return value_on_fail;
    }


    Interpolator1D(Interpolator1D &&f) {
        interpolation = std::move(f.interpolation);
        acc = std::move(f.acc);
    }

    Interpolator1D & operator = (const Interpolator1D &f) {
        if(f.interpolation.get()==nullptr) {
            interpolation.reset(nullptr);
            acc.reset(nullptr);
        } else {
            type = f.type;
            interpolation = interpolation_t (
                gsl_spline_alloc(type,f.interpolation->size),
                [] (gsl_spline*g) {gsl_spline_free(g);}
            );

            if( auto code=gsl_spline_init (interpolation.get(), f.interpolation->x, f.interpolation->y, f.interpolation->size); code!=GSL_SUCCESS )
                throw std::runtime_error(gsl_strerror(code));

            acc = acc_t( gsl_interp_accel_alloc (), [] (gsl_interp_accel*a) {gsl_interp_accel_free(a);} );
        }
        return *this;
    }

    Interpolator1D & operator = (Interpolator1D &&f) {
        if( &f!=this ) {
            interpolation = std::move(f.interpolation);
            acc = std::move(f.acc);
        }
        return *this;
    }

    unsigned Size (void) const {
        return interpolation ? interpolation.get()->size : 0;
    }

    std::pair<double,double> operator [] (unsigned i) const {
        const auto s = interpolation.get();
        if(s and i<s->size)
            return {s->x[i],s->y[i]};
        throw std::invalid_argument("Interpolator1D::operator[]");
    }

    std::string Name (void) const {
        return interpolation ? gsl_spline_name(interpolation.get()) : "(none)";
    }

    double Integral (double a,double b) const {
        if(!interpolation)
            throw std::invalid_argument("Interpolator1D::Integral(): empty curve");
        double result;
        auto status = gsl_spline_eval_integ_e(interpolation.get(),a,b,acc.get(),&result);
        if(status!=GSL_SUCCESS)
            throw std::runtime_error(gsl_strerror(status));
        return result;
    }

    std::span<double> GetX (void) const {return {interpolation->x,interpolation->size};}
    std::span<double> GetY (void) const {return {interpolation->y,interpolation->size};}

    void Print (void) const {
        printf("(x,y) %u points: ",Size());
        for(unsigned i=0;i<Size();i++){
            printf(" (%g,%g)",GetX()[i],GetY()[i]);
        }
        printf("\n");
    }

protected:

    const gsl_interp_type *type;
    interpolation_t interpolation;
    acc_t acc;

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
