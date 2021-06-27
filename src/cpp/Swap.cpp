#include "math.hpp"
#include "YieldCurve.hpp"
#include "Swap.hpp"

float LegFixed::Eval (const YieldCurve &discount_curve) const {
    float rv {0};
    for(int i=0;i<n;i++){
        float t = t0 + (i+1)*dt;
        rv += discount_curve.GetDiscountFactor(t);
    }
    return rv*rate*dt;
}

float LegFloat::Eval (const YieldCurve &discount_curve) const {
    float rv {0};
    for(int i=0;i<n;i++){
        float
            t1 = t0 + i*dt,
            t2 = t1+dt;
        rv += discount_curve.GetDiscountFactor(t2)
              *
              curve->GetForwardRate(t1,t2);
    }
    return rv*dt;
}

// void Swap::AddToCurve (YieldCurve &curve) const {
//     auto t = GetMaturity();
//     curve.CheckMaturityIncreasing(t);
//     // FIXME: initial value?!
//     const float rate_minmax = 0.5; // rates range: -50%...+50%
//     auto result = math::solver(
//         [&] (double x) {
//             curve.SetYield(t,x);
//             auto v =  Eval(curve) - PV();
//             debug("{} x={} v={}",__PRETTY_FUNCTION__,x,v);
//             return v;
//         },
//         math::Parameter(0,0.01,-rate_minmax,rate_minmax),
//         math::Options()
//     );
//     if(!result.IsGood())
//         throw std::runtime_error( fmt::format(
//             "{} {}",
//             __PRETTY_FUNCTION__,result.GetError()
//         ));
//     curve.SetYield(t,result.x.at(0));
// }

// When a swap is added to the curve, there are several possible
// configuarations regartding the float leg:
// (1) the float leg is independent from the discouning curve
//     (multiple curve approach)
//     => no extra code is needed in this case!
// (2) single curve: the float leg is the same as the discount leg
