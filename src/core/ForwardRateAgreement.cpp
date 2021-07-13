#include "YieldCurve.hpp"
#include "ForwardRateAgreement.hpp"

float ForwardRateAgreement::Eval (const YieldCurve &curve) const {
    return -std::log(curve.GetDiscountFactor(start,start+length))/length;
}

void ForwardRateAgreement::AddToCurve (YieldCurve &curve) const {

    if(curve.GetType()==math::Interpolator1D::None)
        curve = math::Interpolator1D (
            std::vector<double>(),
            std::vector<double>(),
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
    if( not (t1<=t2 and t2<t3)){
        printf("%s: t1=%g t2=%g t3=%g\n",__PRETTY_FUNCTION__,t1,t2,t3);
        throw std::invalid_argument(__PRETTY_FUNCTION__);
    }

    const float
        r12_t12 = empty_curve ? 0 : curve.Integral(t1,t2),
        r13 = rate,
        t23 = t3-t2,
        t13 = t3-t1,
        r23 = (r13*t13-r12_t12)/t23;

        // printf("tttt %g %g %g\n",t1,t2,t3);
        // printf("r:  t13=%g r12_t12=%g   r13=%g r23=%g\n",t13,r12_t12,r13,r23);

    curve.SetYield(t3,r23);
}
