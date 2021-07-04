#include "YieldCurve.hpp"
#include "ForwardRateAgreement.hpp"

float ForwardRateAgreement::Eval (const YieldCurve &curve) const {
    return -std::log(curve.GetDiscountFactor(start,start+length))/length;
}

void ForwardRateAgreement::AddToCurve (YieldCurve &curve) const {

    if(curve.GetType()==math::Interpolator1D::None)
        curve = math::Interpolator1D (
            std::span<double>(),
            std::span<double>(),
            math::Interpolator1D::Type::PiecewiseConstant
        );
    if(curve.GetType()!=math::Interpolator1D::PiecewiseConstant)
        throw std::invalid_argument(
            "ForwardRateAgreement::AddToCurve(): only math::Interpolator1D::PiecewiseConstant type is supported."
        );

    const bool
        empty_curve = curve.GetSize()==0;
    const float
        t1 = start,
        t2 = empty_curve ? t1 : curve.GetMaturity(),
        t3 = GetMaturity();

    // curve.CheckMaturityIncreasing(GetMaturity());
    // const bool empty = curve.GetYields().empty();
    if( not (t1<=t2 and t2<t3))
        throw std::invalid_argument(__PRETTY_FUNCTION__);

    const float
        r12 = empty_curve ? 0 : curve.GetForwardRate(t1,t2),
        r13 = rate,
        t12 = t2-t1,
        t23 = t3-t2,
        t13 = t12+t23,
        r23 = (r13*t13-r12*t12)/t23;

        // printf("tttt %g %g %g\n",t1,t2,t3);
        // printf("r:  %g %g   %g %g %g\n",t12,t13,r12,r13,r23);

    curve.SetYield(t3,r23);
}
