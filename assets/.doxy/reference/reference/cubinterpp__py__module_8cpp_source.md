

# File cubinterpp\_py\_module.cpp

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**cubinterpp\_py\_module.cpp**](cubinterpp__py__module_8cpp.md)

[Go to the documentation of this file](cubinterpp__py__module_8cpp.md)


```C++
#include <vector>
#include <pybind11/stl.h>
#include <pybind11/pybind11.h>
#include <cubinterpp.hpp>

namespace py = pybind11;

using DoubleVector = std::vector<double>;
using DoubleVector2 = std::vector<DoubleVector>;
using DoubleVector3 = std::vector<DoubleVector2>;


PYBIND11_MODULE(cubinterpp_py, m) {
    m.doc() = "pybind11 1D cubic and linear interpolation"; // optional module docstring

    py::class_<cip::LinearInterp1D<double>>(m, "LinearInterp1D")
        .def(py::init<DoubleVector, DoubleVector>())
        .def("eval", &cip::LinearInterp1D<double>::eval, py::return_value_policy::reference_internal)
        .def("evaln", &cip::LinearInterp1D<double>::evaln, py::return_value_policy::reference_internal);

    py::class_<cip::MonotonicCubicInterp1D<double>>(m, "MonotonicCubicInterp1D")
        .def(py::init<DoubleVector, DoubleVector>())
        .def("eval", &cip::MonotonicCubicInterp1D<double>::eval, py::return_value_policy::reference_internal)
        .def("evaln", &cip::MonotonicCubicInterp1D<double>::evaln, py::return_value_policy::reference_internal);

    py::class_<cip::MakimaCubicInterp1D<double>>(m, "MakimaCubicInterp1D")
        .def(py::init<DoubleVector, DoubleVector>())
        .def("eval", &cip::MakimaCubicInterp1D<double>::eval, py::return_value_policy::reference_internal)
        .def("evaln", &cip::MakimaCubicInterp1D<double>::evaln, py::return_value_policy::reference_internal);

    py::class_<cip::NaturalCubicInterp1D<double>>(m, "NaturalCubicInterp1D")
        .def(py::init<DoubleVector, DoubleVector>())
        .def("eval", &cip::NaturalCubicInterp1D<double>::eval, py::return_value_policy::reference_internal)
        .def("evaln", &cip::NaturalCubicInterp1D<double>::evaln, py::return_value_policy::reference_internal);

    py::class_<cip::NaturalPeriodicCubicInterp1D<double>>(m, "NaturalPeriodicCubicInterp1D")
        .def(py::init<DoubleVector, DoubleVector>())
        .def("eval", &cip::NaturalPeriodicCubicInterp1D<double>::eval, py::return_value_policy::reference_internal)
        .def("evaln", &cip::NaturalPeriodicCubicInterp1D<double>::evaln, py::return_value_policy::reference_internal);

    py::class_<cip::LinearInterp2D<double>>(m, "LinearInterp2D")
        .def(py::init<DoubleVector, DoubleVector, DoubleVector2>())
        .def("eval", &cip::LinearInterp2D<double>::eval<double, double>, py::return_value_policy::reference_internal)
        .def("evaln", &cip::LinearInterp2D<double>::evaln<DoubleVector, DoubleVector>, py::return_value_policy::reference_internal);

    py::class_<cip::LinearInterp3D<double>>(m, "LinearInterp3D")
        .def(py::init<DoubleVector, DoubleVector, DoubleVector, DoubleVector3>())
        .def("eval", &cip::LinearInterp3D<double>::eval<double, double, double>, py::return_value_policy::reference_internal)
        .def("evaln", &cip::LinearInterp3D<double>::evaln<DoubleVector, DoubleVector, DoubleVector>, py::return_value_policy::reference_internal);

    py::class_<cip::MonotonicCubicInterp2D<double>>(m, "MonotonicCubicInterp2D")
        .def(py::init<DoubleVector, DoubleVector, DoubleVector2>())
        .def("eval", &cip::MonotonicCubicInterp2D<double>::eval<double, double>, py::return_value_policy::reference_internal);

    py::class_<cip::MakimaCubicInterp2D<double>>(m, "MakimaCubicInterp2D")
        .def(py::init<DoubleVector, DoubleVector, DoubleVector2>())
        .def("eval", &cip::MakimaCubicInterp2D<double>::eval<double, double>, py::return_value_policy::reference_internal);

    py::class_<cip::NaturalCubicInterp2D<double>>(m, "NaturalCubicInterp2D")
        .def(py::init<DoubleVector, DoubleVector, DoubleVector2>())
        .def("eval", &cip::NaturalCubicInterp2D<double>::eval<double, double>, py::return_value_policy::reference_internal);

    py::class_<cip::NaturalPeriodicCubicInterp2D<double>>(m, "NaturalPeriodicCubicInterp2D")
        .def(py::init<DoubleVector, DoubleVector, DoubleVector2>())
        .def("eval", &cip::NaturalPeriodicCubicInterp2D<double>::eval<double, double>, py::return_value_policy::reference_internal);

    py::class_<cip::NaturalCubicInterp3D<double>>(m, "NaturalCubicInterp3D")
        .def(py::init<DoubleVector, DoubleVector, DoubleVector, DoubleVector3>())
        .def("eval", &cip::NaturalCubicInterp3D<double>::eval<double, double, double>, py::return_value_policy::reference_internal);

}
```


