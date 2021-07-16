#include "Curve.hpp"
#include "ZeroCouponBond.hpp"

float ZeroCouponBond::Eval (const Curve &curve) const {
    return curve.GetDiscountFactor(GetMaturity());
}

void ZeroCouponBond::AddToCurve (Curve &curve) const {
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
        t = GetMaturity(),
        dt = empty_curve ? t : (t-curve.GetMaturity());
    if(not(dt>0))
        throw std::invalid_argument("ZeroCouponBond::AddToCurve: bad instrument maturity");
    const float
        last_point_df = empty_curve ? 1 : curve.GetDiscountFactor(curve.GetMaturity());

//     // auto it = curve.GetYields().rbegin();
//     // auto last_point_df = it==curve.GetYields().rend() ?
//     //     1 : curve.GetDiscountFactor(it->first);
//     // auto dt = it==curve.GetYields().rend() ?
//     //     GetMaturity().GetYearFraction() : (t-it->first).GetYearFraction();
//     // if(not(dt>0))
//     //     throw std::runtime_error(__PRETTY_FUNCTION__);
    curve.SetYield(t,-std::log(Value()/last_point_df)/dt);
}
