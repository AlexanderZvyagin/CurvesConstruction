#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>

#include "math.hpp"
#include "ZeroCouponBond.hpp"
#include "ForwardRateAgreement.hpp"
#include "Swap.hpp"
#include "YieldCurve.hpp"

using namespace pybind11::literals;
namespace py = pybind11;

PYBIND11_MODULE(curves, m) {

    py::class_<math::Interpolator1D> i1d (m, "Interpolator1D");
    i1d
        .def(py::init<>())
    ;

    py::enum_<math::Interpolator1D::Type>(i1d, "Type")
        .value("None",                  math::Interpolator1D::Type::None)
        .value("PiecewiseConstant",     math::Interpolator1D::Type::PiecewiseConstant)
        .value("Linear",                math::Interpolator1D::Type::Linear)
        .value("Polynomial",            math::Interpolator1D::Type::Polynomial)
        .value("CubicSpline",           math::Interpolator1D::Type::CubicSpline)
        .value("CubicSplinePeriodic",   math::Interpolator1D::Type::CubicSplinePeriodic)
        .value("Akima",                 math::Interpolator1D::Type::Akima)
        .value("AkimaPeriodic",         math::Interpolator1D::Type::AkimaPeriodic)
        .value("Steffen",               math::Interpolator1D::Type::Steffen)
        .export_values()
    ;

    /////////

    py::class_<LegFixed> (m, "LegFixed")
        .def(py::init<>())
        .def(py::init<float,float,int,float>())
        .def("Eval", &LegFixed::Eval)
        .def("Value", &LegFixed::Value)
        .def("__repr__", &LegFixed::About)
    ;

    py::class_<LegFloat> (m, "LegFloat")
        .def(py::init<>())
        .def(py::init<float,float,int,YieldCurve&>())
        .def("Eval", &LegFloat::Eval)
        .def("Value", &LegFloat::Value)
        .def("__repr__", &LegFloat::About)
        // .def("__repr__", [] (const LegFloat &v) {return v.About();})
    ;

    py::class_<Swap> (m, "Swap")
        .def(py::init<>())
        .def_readwrite("lfix", &Swap::lfix)
        .def_readwrite("lflt", &Swap::lflt)
        .def("Eval", &Swap::Eval)
        .def("Value", &Swap::Value)
        .def("AddToCurve",&Swap::AddToCurve)
        .def("__repr__", &Swap::About)
        // .def("__repr__", [] (const Swap &v) {return v.About();})
    ;

    py::class_<ZeroCouponBond> (m, "ZeroCouponBond")
        .def(py::init<float,float>())
        .def("Eval", &ZeroCouponBond::Eval)
        .def("Value", &ZeroCouponBond::Value)
        .def("AddToCurve",&ZeroCouponBond::AddToCurve)
        .def("__repr__", &ZeroCouponBond::About)
    ;

    py::class_<ForwardRateAgreement> (m, "ForwardRateAgreement")
        .def(py::init<float,float,float>())
        .def("Eval", &ForwardRateAgreement::Eval)
        .def("Value", &ForwardRateAgreement::Value)
        .def("AddToCurve",&ForwardRateAgreement::AddToCurve)
        .def("__repr__", &ForwardRateAgreement::About)
    ;

    py::class_<YieldCurve> (m, "YieldCurve")
        .def(py::init<>())
        .def("__repr__", [] (const YieldCurve &c) {return "YieldCurve";})
        .def("__call__", [] (const YieldCurve &c,float v) {return c(v);})
        .def("SetYield", &YieldCurve::SetYield)
        .def("GetDiscountFactor", [] (const YieldCurve &c,float t1) {return c.GetDiscountFactor(t1);})
        .def("GetDiscountFactor", [] (const YieldCurve &c,float t1,float t2) {return c.GetDiscountFactor(t1,t2);})
        .def("Add",[] (YieldCurve &c,const ZeroCouponBond &v) {return c.Add(v);})
        .def("Add",[] (YieldCurve &c,const ForwardRateAgreement &v) {return c.Add(v);})
        .def("Add",[] (YieldCurve &c,const Swap &v) {return c.Add(v);})
        .def("GetForwardRate", &YieldCurve::GetForwardRate)
        .def("GetMaturity", &YieldCurve::GetMaturity)
        .def("Print", &YieldCurve::Print)
        .def("Build",
            [] (
                YieldCurve &c,
                math::Interpolator1D::Type itype,
                float rate_extrapolation)
            {
                return c.Build(itype,rate_extrapolation);
            },
            py::arg("itype") = math::Interpolator1D::Type::CubicSpline,
            py::arg("rate_extrapolation") = 0
        )
        .def("GetInstruments",&YieldCurve::GetInstruments)
    ;

    // py::class_<math::Options> (m, "MathOptions")
    //     .def(py::init<>())
    //     .def_readwrite("eps_abs", &math::Options::eps_abs)
    //     .def_readwrite("eps_rel", &math::Options::eps_rel)
    //     .def("__repr__", [] (const math::Options &v) {return fmt::format("{}",v);});
    // ;
    // py::class_<math::Stat> (m, "Stat")
    //     .def(py::init<>())
    //     .def_readwrite("n", &math::Stat::n)
    //     .def_readwrite("mom1", &math::Stat::mom1)
    //     .def_readwrite("mom2", &math::Stat::mom2)
    //     .def          ("N",    &math::Stat::N)
    //     .def          ("Mean",     &math::Stat::Mean)
    //     .def          ("Variance", &math::Stat::Variance)
    //     .def          ("Sigma",    &math::Stat::Sigma)
    //     .def          ("MeanError",&math::Stat::MeanError)
    //     .def("__repr__", [] (const math::Stat &v) {return fmt::format("{}",v);});
    // ;
    // py::class_<math::Interpolator1D> (m, "Interpolator1D")
    //     .def(py::init<>())
    //     .def(py::init<std::vector<double>,std::vector<double>>())
    //     .def("__call__",      &math::Interpolator1D::operator ())
    //     .def("__repr__", [] (const math::Interpolator1D &v) {return fmt::format("{}",v);})
    //     .def("__len__", []  (const math::Interpolator1D &v) {return v.Size();} )
    //     .def("__getitem__", []  (const math::Interpolator1D &v,int i) {return v[i];} )
    // ;
}
