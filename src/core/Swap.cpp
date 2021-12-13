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

void Swap::AddToCurve (YieldCurve &curve) const {
    if(curve.GetType()==math::Interpolator1D::None)
        curve = math::Interpolator1D (
            std::vector<double>(),
            std::vector<double>(),
            math::Interpolator1D::Type::PiecewiseConstant
        );
    if(curve.GetType()!=math::Interpolator1D::PiecewiseConstant)
        throw std::invalid_argument(
            "Swap::AddToCurve(): only math::Interpolator1D::PiecewiseConstant type is supported."
        );

    if(lflt.curve!=&curve)
        throw std::invalid_argument(
            "Swap::AddToCurve(): single curve approach is coded."
        );

    float
        t = GetMaturity(),
        rate_minmax = 0.2; // FIXME: constant in the code
    auto result = math::solver(
        [&] (double x) {
            curve.SetYield(t,x);
            auto v =  Eval(curve) - Value();
            // printf("%s x=%g v=%g\n",__PRETTY_FUNCTION__,x,v);
            return v;
        },
        math::Parameter(0,0.01,-rate_minmax,rate_minmax),
        math::Options()
    );

    if(!result.IsGood())
        throw std::runtime_error(result.GetError());
    curve.SetYield(t,result.x.at(0));
}
