#include "cubic_spline.hpp"

#include <pybind11/pybind11.h>
namespace py = pybind11;

//* with STL vector
#include <pybind11/stl.h>
#include <vector>
using DoubleVector = std::vector<double>;
//*/

/* with Eigen vector
#include <pybind11/eigen.h>
#include <Eigen/Core>
using namespace Eigen;
using DoubleVector = Eigen::VectorXd;
//*/

PYBIND11_MODULE(cubic_spline, m) {
    m.doc() = "pybind11 1D cubic splines"; // optional module docstring
    py::class_<cbs::MonotonicSpline1D<DoubleVector>>(m, "MonotonicSpline1D")
        .def(py::init<DoubleVector, DoubleVector>())
        .def("eval", &cbs::MonotonicSpline1D<DoubleVector>::eval, py::return_value_policy::reference_internal)
        .def("evaln", &cbs::MonotonicSpline1D<DoubleVector>::evaln, py::return_value_policy::reference_internal);

    py::class_<cbs::AkimaSpline1D<DoubleVector>>(m, "AkimaSpline1D")
        .def(py::init<DoubleVector, DoubleVector>())
        .def("eval", &cbs::AkimaSpline1D<DoubleVector>::eval, py::return_value_policy::reference_internal)
        .def("evaln", &cbs::AkimaSpline1D<DoubleVector>::evaln, py::return_value_policy::reference_internal);

    py::class_<cbs::NaturalSpline1D<DoubleVector>>(m, "NaturalSpline1D")
        .def(py::init<DoubleVector, DoubleVector>())
        .def("eval", &cbs::NaturalSpline1D<DoubleVector>::eval, py::return_value_policy::reference_internal)
        .def("evaln", &cbs::NaturalSpline1D<DoubleVector>::evaln, py::return_value_policy::reference_internal);
}
