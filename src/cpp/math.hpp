#pragma once

#include <functional>
#include <cmath>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <memory>

#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_integration.h>

#include "gsl_init.hpp"

namespace math {

struct Parameter {
    double value, error, min, max;
    Parameter (double value=NAN, double error=NAN, double min=NAN, double max=NAN)
        : value (value)
        , error (error)
        , min   (min)
        , max   (max)
    {}

    operator double (void) const {return value;}
};

struct Options {
    Options (void) : eps_abs(1e-6), eps_rel(1e-5), limit(10000), iters(100) {}
    double eps_abs, eps_rel;
    unsigned limit;
    unsigned iters;
};

inline
std::ostream & operator << (std::ostream &os, const Options &v) {
    os << "(math options: eps_abs="
       << v.eps_abs
       << " eps_rel="
       << v.eps_rel
       << ")";
    return os;
}

struct Result {
    double value {NAN}, error {NAN};
    unsigned calls;
    int code;
    std::vector <Parameter> x;
    std::string error_text;

    bool IsGood (void) const {return error_text.empty();}
    operator bool (void) const {return IsGood();}
    void SetError (std::string error) {error_text=error;}
    std::string GetError (void) const {return error_text;}

    double ValueOr (const std::string &msg) {
        if(!std::isnan(value)) return value;
        throw std::runtime_error(gsl_strerror(code));
    }

    double ValueOr (double defval) {
        return std::isnan(value) ? defval : value;
    }
};

inline
std::ostream & operator << (std::ostream &os, const Result &r) {
    os << "value=" << r.value << " ";
    os << "error=" << r.error << " ";
    os << "code=" << r.code << " ";
    os << "error=\"" << r.error_text << "\" ";
    if(!r.x.empty()){
        os << "x=[";
        for(auto x: r.x) os << x << ",";
        os << "]";
    }
    return os;
}

template <typename F>
struct Wrapper {
    F cpp_f;
    gsl_function gsl_f;
    Wrapper (F f) : cpp_f(f) {
        gsl_f.function = Call;
        gsl_f.params = this;
    }
    static double Call (double x, void *pars) {
        return reinterpret_cast <Wrapper*> (pars) -> cpp_f(x);
    }
};

Result integral (
    double (*f) (double, void*),
    void *pars,
    std::vector <double> points,
    Options opts = {}
);

template <typename F>
Result integral (
    F f,
    std::vector <double> points,
    Options opts = {}
){
    Wrapper<F> w (f);
    return integral (&w.Call,&w,points,opts);
}

inline
std::vector <double>
linspace (unsigned n,double min,double max) {
    switch(n) {
        case 0: return {};
        case 1: if(min==max) return {min}; else throw std::invalid_argument("linspace: n=1 and min!=max");
        default: {
            std::vector<double> v(n,NAN);
            v[0] = min;
            for( unsigned i=1; i<n-1; i++)
                v[i] = min + (max-min)* i/double(n-1);
            v[n-1] = max;
            return v;
        }
    }
}

Result solver (
    double (*f) (double, void*),
    void *data,
    const Parameter &p,
    Options opts = Options {}
);

template <typename F>
Result solver (
    F f,
    const Parameter &p,
    const Options &opts = Options {}
){
    Wrapper<F> w {f};
    return solver (&w.Call, &w, p, opts);
}

// axx + bx + c = 0
inline
std::pair <double, double>
quadratic_equation_roots (double a, double b, double c) {
    const double
        D  = std::sqrt (b*b-4*a*c),
        x1 = (-b+D) / (2*a),
        x2 = (-b-D) / (2*a);
    return std::make_pair (x1, x2);
}

}
