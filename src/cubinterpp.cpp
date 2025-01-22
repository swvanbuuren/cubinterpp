#include "cubic_spline.hpp"
#include "linear_interp.hpp"

#include <pybind11/pybind11.h>
namespace py = pybind11;

//* with STL vector
#include <pybind11/stl.h>
#include <vector>
using DoubleVector = std::vector<double>;
using DoubleVector2 = std::vector<DoubleVector>;
using DoubleVector3 = std::vector<DoubleVector2>;


PYBIND11_MODULE(cubinterpp_py, m) {
    m.doc() = "pybind11 1D cubic and linear interpolation"; // optional module docstring

    py::class_<cip::LinearInterp1D<double>>(m, "LinearInterp1D")
        .def(py::init<DoubleVector, DoubleVector>())
        .def("eval", &cip::LinearInterp1D<double>::eval, py::return_value_policy::reference_internal)
        .def("evaln", &cip::LinearInterp1D<double>::evaln, py::return_value_policy::reference_internal);

    py::class_<cip::MonotonicSpline1D<double>>(m, "MonotonicSpline1D")
        .def(py::init<DoubleVector, DoubleVector>())
        .def("eval", &cip::MonotonicSpline1D<double>::eval, py::return_value_policy::reference_internal)
        .def("evaln", &cip::MonotonicSpline1D<double>::evaln, py::return_value_policy::reference_internal);

    py::class_<cip::AkimaSpline1D<double>>(m, "AkimaSpline1D")
        .def(py::init<DoubleVector, DoubleVector>())
        .def("eval", &cip::AkimaSpline1D<double>::eval, py::return_value_policy::reference_internal)
        .def("evaln", &cip::AkimaSpline1D<double>::evaln, py::return_value_policy::reference_internal);

    py::class_<cip::NaturalSpline1D<double>>(m, "NaturalSpline1D")
        .def(py::init<DoubleVector, DoubleVector>())
        .def("eval", &cip::NaturalSpline1D<double>::eval, py::return_value_policy::reference_internal)
        .def("evaln", &cip::NaturalSpline1D<double>::evaln, py::return_value_policy::reference_internal);

    py::class_<cip::LinearInterp2D<double>>(m, "LinearInterp2D")
        .def(py::init<DoubleVector, DoubleVector, DoubleVector2>())
        .def("eval", &cip::LinearInterp2D<double>::eval<double, double>, py::return_value_policy::reference_internal)
        .def("evaln", &cip::LinearInterp2D<double>::evaln<DoubleVector, DoubleVector>, py::return_value_policy::reference_internal);

    py::class_<cip::LinearInterp3D<double>>(m, "LinearInterp3D")
        .def(py::init<DoubleVector, DoubleVector, DoubleVector, DoubleVector3>())
        .def("eval", &cip::LinearInterp3D<double>::eval<double, double, double>, py::return_value_policy::reference_internal)
        .def("evaln", &cip::LinearInterp3D<double>::evaln<DoubleVector, DoubleVector, DoubleVector>, py::return_value_policy::reference_internal);

}
