#include "YieldCurve.hpp"
#include "ZeroCouponBond.hpp"

float ZeroCouponBond::Eval (const YieldCurve &curve) const {
    return curve.GetDiscountFactor(GetMaturity());
}

// YieldCurve ZeroCouponBond::AddToCurve (const YieldCurve &curve) const {
//     printf("%s: no code\n",__PRETTY_FUNCTION__);
//     // auto t = GetMaturity();
//     // curve.CheckMaturityIncreasing(t);
//     // auto it = curve.GetYields().rbegin();
//     // auto last_point_df = it==curve.GetYields().rend() ?
//     //     1 : curve.GetDiscountFactor(it->first);
//     // auto dt = it==curve.GetYields().rend() ?
//     //     GetMaturity().GetYearFraction() : (t-it->first).GetYearFraction();
//     // if(not(dt>0))
//     //     throw std::runtime_error(__PRETTY_FUNCTION__);
//     // curve.SetYield(t,-std::log(value/last_point_df)/dt);
// }
