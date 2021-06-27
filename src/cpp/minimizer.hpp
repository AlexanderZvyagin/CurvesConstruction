#pragma once

#include <cmath>
#include <cstdio>
#include <span>

#include "math.hpp"
#include <gsl/gsl_multimin.h>

namespace math {

template <typename F>
struct WrapperForMinimizer {
    static
    double for_gsl_minimization (const gsl_vector *v, void *data) {
        return reinterpret_cast <WrapperForMinimizer*> (data) -> cpp_f(std::span<double>(v->data,v->size));
    }
    F cpp_f;

    // gsl_function gsl_f;
    WrapperForMinimizer (F f) : cpp_f(f) {}
    //     gsl_f.function = Call;
    //     gsl_f.params = this;
    // }
//
//
//
//     // static double Call (double x, void *pars) {
//     //     return reinterpret_cast <Wrapper*> (pars) -> cpp_f(x);
//     // }
};

//
// class Minimizer {
// protected:
//     static double for_gsl_minimization (const gsl_vector *v, void *data) {
//         auto self = reinterpret_cast<Minimizer*> (data);
//         return
//     }
// public:
//
// };




template <typename F>
Result minimize (
    F f,
    const std::vector<Parameter> &pars,
    const Options &opts
){
    WrapperForMinimizer w(f);

    gsl_multimin_function fmw;
    fmw.n = pars.size();
    fmw.f = w.for_gsl_minimization;
    fmw.params = &w;

    std::unique_ptr<gsl_multimin_fminimizer,std::function<void(gsl_multimin_fminimizer*)>>
        s (
            gsl_multimin_fminimizer_alloc (gsl_multimin_fminimizer_nmsimplex2, pars.size()),
            [] (gsl_multimin_fminimizer *p) {gsl_multimin_fminimizer_free(p);}
        );

    std::unique_ptr<gsl_vector,std::function<void(gsl_vector*)>>
        x_start (
            gsl_vector_alloc (pars.size()),
            [] (gsl_vector *p) {gsl_vector_free(p);}
        ),
        x_steps (
            gsl_vector_alloc (pars.size()),
            [] (gsl_vector *p) {gsl_vector_free(p);}
        );

    for(unsigned i=0; i<pars.size(); i++){
        auto par = pars[i];
        gsl_vector_set(x_start.get(),i,par.value);
        gsl_vector_set(x_steps.get(),i,par.error);
    }

    gsl_multimin_fminimizer_set (s.get(), &fmw, x_start.get(), x_steps.get());

    int status;
    unsigned iter {0};
    do {
        iter++;
        status = gsl_multimin_fminimizer_iterate(s.get());
        // debug("Model::Calibrate: iter={} status={}",iter,gsl_strerror(status));

        if(status!=GSL_SUCCESS)
            break;
        double size = gsl_multimin_fminimizer_size (s.get());
        status = gsl_multimin_test_size (size, opts.eps_abs.value());
    } while (status == GSL_CONTINUE && iter < opts.iters.value());

    if(status!=GSL_SUCCESS)
        throw std::runtime_error(gsl_strerror(status));

    Result r;
    for(auto x: std::span{s->x->data,s->x->size})
        r.x.push_back(Parameter(x));
    r.calls = iter;
    r.code = status;
    if(status!=GSL_SUCCESS)
        r.SetError(gsl_strerror(status));
    r.value = s->fval;

    return r;
}


}
