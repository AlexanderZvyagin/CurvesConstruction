#pragma once

#include <cmath>
#include <cstdio>

#include "math.hpp"
#include <gsl/gsl_multimin.h>

namespace math {

template <typename F>
struct WrapperForMinimizer {
    static
    double for_gsl_minimization (const gsl_vector *v, void *data) {
        return reinterpret_cast <WrapperForMinimizer*> (data) -> cpp_f(std::vector<double>(v->data,v->data+v->size));
    }
    F cpp_f;
    WrapperForMinimizer (F f) : cpp_f(f) {}
};

template <typename F>
Result minimize (
    F f,
    const std::vector<Parameter> &pars,
    const Options &opts
){
    WrapperForMinimizer<F> w(f);

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
        status = gsl_multimin_test_size (size, opts.eps_abs);
    } while (status == GSL_CONTINUE && iter < opts.iters);

    // if(status!=GSL_SUCCESS)
    //     throw std::runtime_error(gsl_strerror(status));

    Result r;
    for(size_t i=0; i<s->x->size; i++)
        r.x.push_back(Parameter(s->x->data[i]));
    r.calls = iter;
    r.code = status;
    if(status!=GSL_SUCCESS)
        r.SetError(gsl_strerror(status));
    r.value = s->fval;

    return r;
}

}
