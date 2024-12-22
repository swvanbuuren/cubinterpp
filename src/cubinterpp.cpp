#include "cubic_spline.hpp"
#include "linear_interp.hpp"

#include <pybind11/pybind11.h>
namespace py = pybind11;

//* with STL vector
#include <pybind11/stl.h>
#include <vector>
using DoubleVector = std::vector<double>;
using DoubleVector2 = std::vector<std::vector<double>>;


PYBIND11_MODULE(cubinterpp, m) {
    m.doc() = "pybind11 1D cubic and linear interpolation"; // optional module docstring

    py::class_<cip::LinearInterp1D<double>>(m, "LinearInterp1D")
        .def(py::init<DoubleVector, DoubleVector>())
        .def("eval", &cip::LinearInterp1D<double>::eval, py::return_value_policy::reference_internal)
        .def("evaln", &cip::LinearInterp1D<double>::evaln, py::return_value_policy::reference_internal);

    py::class_<cip::MonotonicSpline1D<DoubleVector>>(m, "MonotonicSpline1D")
        .def(py::init<DoubleVector, DoubleVector>())
        .def("eval", &cip::MonotonicSpline1D<DoubleVector>::eval, py::return_value_policy::reference_internal)
        .def("evaln", &cip::MonotonicSpline1D<DoubleVector>::evaln, py::return_value_policy::reference_internal);

    py::class_<cip::AkimaSpline1D<DoubleVector>>(m, "AkimaSpline1D")
        .def(py::init<DoubleVector, DoubleVector>())
        .def("eval", &cip::AkimaSpline1D<DoubleVector>::eval, py::return_value_policy::reference_internal)
        .def("evaln", &cip::AkimaSpline1D<DoubleVector>::evaln, py::return_value_policy::reference_internal);

    py::class_<cip::NaturalSpline1D<DoubleVector>>(m, "NaturalSpline1D")
        .def(py::init<DoubleVector, DoubleVector>())
        .def("eval", &cip::NaturalSpline1D<DoubleVector>::eval, py::return_value_policy::reference_internal)
        .def("evaln", &cip::NaturalSpline1D<DoubleVector>::evaln, py::return_value_policy::reference_internal);

    py::class_<cip::LinearInterp2D<double>>(m, "LinearInterp2D")
        .def(py::init<DoubleVector, DoubleVector, DoubleVector2>())
        .def("eval", &cip::LinearInterp2D<double>::eval, py::return_value_policy::reference_internal)
        .def("evaln", &cip::LinearInterp2D<double>::evaln, py::return_value_policy::reference_internal);

}
