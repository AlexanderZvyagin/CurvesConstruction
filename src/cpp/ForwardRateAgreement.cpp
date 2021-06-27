#include "YieldCurve.hpp"
#include "ForwardRateAgreement.hpp"

float ForwardRateAgreement::Eval (const YieldCurve &curve) const {
    return -std::log(curve.GetDiscountFactor(start,start+length))/length;
}

// void ForwardRateAgreement::AddToCurve (YieldCurve &curve) const {
//     curve.CheckMaturityIncreasing(GetMaturity());
//     const bool empty = curve.GetYields().empty();
//     const auto
//         t1 = start,
//         t2 = empty ? t1 : curve.GetMaturity(),
//         t3 = GetMaturity();
//     if( not (t1<=t2 and t2<t3))
//         throw std::logic_error(fmt::format("{}: t1<=t2<t3: {} {} {}",__PRETTY_FUNCTION__,t1,t2,t3));
//     const float
//         r12 = curve.GetForwardRate(t1,t2),
//         r13 = rate,
//         t12 = (t2-t1).GetYearFraction(),
//         t23 = (t3-t2).GetYearFraction(),
//         t13 = t12+t23,
//         r23 = (r13*t13-r12*t12)/t23;
//
//     curve.SetYield(t3,r23);
//
//     // the equation is r13*t13 = r12*t12+r23*t23
//
//
//
//     // const auto
//     //     df1 = empty ? 1 : curve.GetDiscountFactor(t1),
//     //     df2 = empty ? 1 : curve.GetDiscountFactor(t2);
//     // fmt::print("t1={} t2={} t3={}\n",t1,t2,t3);
//     // fmt::print("df1={} df2={}\n",df1,df2);
//     // fmt::print("calc: {} {} {}\n",std::log(df2/df1),(t3-t1).ToDouble()*rate,(t3-t2).GetYearFraction());
//     // curve.SetYield( t3,
//     //     std::log(df2/df1) + (t3-t1).ToDouble()*rate
//     //     /
//     //     (t3-t2).GetYearFraction()
//     // );
// }
