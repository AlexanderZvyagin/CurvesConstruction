#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_multimin.h>

#include "math.hpp"
#include "gsl_init.hpp"

namespace math {

Result solver (
    double (*f) (double, void*),
    void *data,
    const Parameter &p,
    Options opts
){
    Result result;
    result.calls = 0;
    result.x.push_back(p);
    Parameter &x = result.x.back();

    gsl_function gsl_f {f,data};

    const gsl_root_fsolver_type *T = gsl_root_fsolver_brent;
    std::unique_ptr<gsl_root_fsolver,std::function<void(gsl_root_fsolver*)>>
        s (gsl_root_fsolver_alloc (T),
            [] (gsl_root_fsolver *p) {gsl_root_fsolver_free(p);});
    gsl_root_fsolver_set (s.get(), &gsl_f, x.min, x.max);

    const auto has_resources = [&] (void) -> bool {return opts.limit ? (result.calls.value()<opts.limit.value()) : true;};

    result.code  = GSL_CONTINUE;
    while (result.code.value()==GSL_CONTINUE and has_resources()) {
        result.calls.value() ++;
        gsl_root_fsolver_iterate (s.get());
        x.min = gsl_root_fsolver_x_lower (s.get());
        x.max = gsl_root_fsolver_x_upper (s.get());
        result.code = gsl_root_test_interval (
            x.min, x.max,
            opts.eps_abs.value(), opts.eps_rel.value()
        );
    }

    x.value = gsl_root_fsolver_root (s.get());
    x.error = x.max - x.min;
    result.calls.value() ++;
    result.value = f(x.value,data);
    if(result.code!=GSL_SUCCESS)
        result.SetError(gsl_strerror(result.code.value()));

    return result;
}

Result integral (
    double (*f) (double, void*),
    void *pars,
    std::vector <double> points,
    Options opts
){
    std::unique_ptr<
        gsl_integration_workspace,
        std::function <void(gsl_integration_workspace*)>
    > workspace (
        gsl_integration_workspace_alloc (opts.limit.value()),
        [] (gsl_integration_workspace *w) {gsl_integration_workspace_free(w);}
    );
    gsl_function gsl_f {f,pars};
    Result r;
    r.value = r.error = NAN;
    r.code = gsl_integration_qagp (
        &gsl_f,
        points.data(),
        points.size(),
        opts.eps_abs.value(),
        opts.eps_rel.value(),
        opts.limit.value(),
        workspace.get(),
        &r.value.value(),
        &r.error.value()
    );
    return r;
}

} // namespace
