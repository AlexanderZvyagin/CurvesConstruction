#include <sstream>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>

#include "math.hpp"
#include "ZeroCouponBond.hpp"
#include "ForwardRateAgreement.hpp"
#include "Swap.hpp"
#include "Curve.hpp"

using namespace pybind11::literals;
namespace py = pybind11;

PYBIND11_MODULE(curves, m) {

    py::class_<math::Interpolator1D> i1d (m, "Interpolator1D");
    i1d
        .def(py::init<>())
    ;

    py::class_<math::Options> (m, "MathOptions")
        .def(py::init<>())
        .def_readwrite("eps_abs", &math::Options::eps_abs)
        .def_readwrite("eps_rel", &math::Options::eps_rel)
        .def_readwrite("limit", &math::Options::limit)
        .def_readwrite("iters", &math::Options::iters)
        .def("__repr__", [] (const math::Options &v) {std::stringstream s; s<<v; return s.str();});
    ;

    py::class_<math::Result> (m, "MathResult")
        .def(py::init<>())
        .def_readwrite("value", &math::Result::value)
        .def_readwrite("error", &math::Result::error)
        .def_readwrite("calls", &math::Result::calls)
        .def_readwrite("code", &math::Result::code)
        .def_readwrite("x", &math::Result::x)
        .def_readwrite("error_text", &math::Result::error_text)
        .def("__repr__", [] (const math::Result &v) {std::stringstream s; s<<v; return s.str();});
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
        .def(py::init<float,float,int,Curve&>())
        .def("Eval", &LegFloat::Eval)
        .def("Value", &LegFloat::Value)
        .def("__repr__", &LegFloat::About)
        // .def("__repr__", [] (const LegFloat &v) {return v.About();})
    ;

    // py::class_<Instrument> (m, "Instrument")
    //     .def(py::init<>())
    //     .def("Eval", &Swap::Eval)
    //     .def("Value", &Swap::Value)
    //     .def("AddToCurve",&Swap::AddToCurve)
    //     .def("__repr__", &Swap::About)
    // ;

    py::class_<Swap> (m, "Swap")
        .def(py::init<>())
        .def_readwrite("lfix", &Swap::lfix)
        .def_readwrite("lflt", &Swap::lflt)
        .def("GetMaturity", &Swap::GetMaturity)
        .def("Eval", &Swap::Eval)
        .def("Value", &Swap::Value)
        .def("AddToCurve",&Swap::AddToCurve)
        .def("__repr__", &Swap::About)
        // .def("__repr__", [] (const Swap &v) {return v.About();})
    ;

    py::class_<ZeroCouponBond> (m, "ZeroCouponBond")
        .def(py::init<float,float>())
        .def("GetMaturity", &ZeroCouponBond::GetMaturity)
        .def("Eval", &ZeroCouponBond::Eval)
        .def("Value", &ZeroCouponBond::Value)
        .def("AddToCurve",&ZeroCouponBond::AddToCurve)
        .def("__repr__", &ZeroCouponBond::About)
    ;

    py::class_<ForwardRateAgreement> (m, "ForwardRateAgreement")
        .def(py::init<float,float,float>())
        .def("GetMaturity", &ForwardRateAgreement::GetMaturity)
        .def("Eval", &ForwardRateAgreement::Eval)
        .def("Value", &ForwardRateAgreement::Value)
        .def("AddToCurve",&ForwardRateAgreement::AddToCurve)
        .def("__repr__", &ForwardRateAgreement::About)
    ;

    py::class_<Curve> (m, "Curve")
        .def(py::init<>())
        .def("__repr__", [] (const Curve &c) {std::stringstream s; s<<c; return s.str();})
        .def("__call__", [] (const Curve &c,float v) {return c(v);})
        .def("SetYield", &Curve::SetYield)
        .def("GetDiscountFactor", [] (const Curve &c,float t1) {return c.GetDiscountFactor(t1);})
        .def("GetDiscountFactor", [] (const Curve &c,float t1,float t2) {return c.GetDiscountFactor(t1,t2);})
        .def("Add",[] (Curve &c,const ZeroCouponBond &v) {return c.Add(v);})
        .def("Add",[] (Curve &c,const ForwardRateAgreement &v) {return c.Add(v);})
        .def("Add",[] (Curve &c,const Swap &v) {return c.Add(v);})
        .def("GetForwardRate", &Curve::GetForwardRate)
        .def("GetMaturity", &Curve::GetMaturity)
        // .def("Print", &Curve::Print)
        .def("Integral", &Curve::Integral)
        .def("GetType", [] (const Curve &c) {return c.GetType();})
        .def("GetType", [] (const Curve &c,const std::string &name) {return Curve::GetType(name);})
        .def("GetTypeName", &Curve::GetTypeName)
        .def("GetSize", &Curve::GetSize)
        .def("GetX", &Curve::GetX)
        .def("GetY", &Curve::GetY)
        .def("Build",
            [] (
                Curve &c,
                math::Interpolator1D::Type itype
            ) {
                return c.Build(itype);
            },
            py::arg("itype") = math::Interpolator1D::Type::CubicSpline
        )
        .def("Build",
            [] (
                Curve &c,
                math::Interpolator1D::Type itype,
                math::Options opts
            ) {
                return c.Build(itype,opts);
            }
        )
        .def("GetInstruments",
            [] (const Curve &c){
                std::vector<std::unique_ptr<Instrument>> v;
                for(std::map <float,std::shared_ptr<Instrument>>::const_iterator it=c.GetInstruments().cbegin(); it!=c.GetInstruments().cend(); it++)
                    v.emplace_back(it->second->Clone());
                return v;
            }
        )
    ;

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
