#include "YieldCurve.hpp"
#include "Instrument.hpp"
#include "minimizer.hpp"

YieldCurve & YieldCurve::Add (const Instrument &x) {
    instruments[x.GetMaturity()].reset(x.Clone());
    return *this;
}

// YieldCurve & YieldCurve::Build (void) {
//     yields.clear();
//     for(auto &[t,instr]: instruments){
//         instr->AddToCurve(*this);
//     }
//     return *this;
// }

YieldCurve & YieldCurve::Build (const gsl_interp_type *_type,float yield_to_infinity) {
    // grid on time
    std::vector<double> vx {0}; // always start with t=0 point
    for(auto [t,instr]: GetInstruments()) {
        if(t>0)
            vx.push_back(t);
        else
            throw std::invalid_argument("YieldCurve::Build: instrument maturity must be >1");
    }

    std::vector<math::Parameter> pars (vx.size()-1,math::Parameter(0,1e-2));

    printf("x=[");
    for(auto x: vx)
        printf("%g,",x);
    printf("]\n");

    // const float yield_to_infinity = 0; // FIXME

    auto func = [&] (const std::span<double> pars) -> double {
        // printf("y=[");
        // for(auto x: pars)
        //     printf("%g,",x);
        // printf("]\n");

        std::vector<double> vy(pars.begin(),pars.end());
        vy.push_back(yield_to_infinity);


        // YieldCurve curve(std::span<double>(vx.data(),vx.size()),pars,_type);
        // printf("%d %d\n",vx.size(),pars.size());
        YieldCurve curve (
            std::span<double>(vx.data(),vx.size()),
            std::span<double>(vy.data(),vy.size()),
            _type
        );
        // printf("ok!\n");

        // ft();
        // printf("C %g %g %g\n",curve.GetDiscountFactor(0),curve.GetDiscountFactor(3),curve.GetDiscountFactor(5));

        double result {0};
        for(auto &[t,instr]: instruments) {
            // printf("+= %g %g\n",instr->Value(),instr->Eval(curve));
            result += std::pow( instr->Value()-instr->Eval(curve), 2 );
        }
        // printf("result=%g\n",result);
        return result;
    };

    math::Options opts;
    opts.iters = 1000;
    auto r = math::minimize(
        func,
        pars,
        opts
    );

    std::cout << r << "\n";

    std::vector<double> vy;
    for( auto y: r.x)
        vy.push_back(y);
    vy.push_back(yield_to_infinity);

    if(vx.size()!=vy.size())
        throw std::logic_error("YieldCurve::Build: internal error");

    *this = math::Interpolator1D(
        std::span<double>(vx.data(),vx.size()),
        std::span<double>(vy.data(),vy.size()),
        _type);

    return *this;
}


void YieldCurve::Print(void) const {
    printf("YieldCurve interpolation type: %s\n",Name().c_str());
    // for( auto [t,y]: yields) {
    //     printf("  t={}  yield={}\n",t,y);
    // }

    for(auto [t,instr]: GetInstruments()) {
        if(!instr) continue;
        printf("%s\n",instr->About().c_str());
        printf("    market quote ... %g\n",instr->Value());
        printf("    caluclated ..... %g\n",instr->Eval(*this));
        printf("    difference ..... %g\n",instr->Value()-instr->Eval(*this));
        printf("\n");
    }
}
