# cubinterpp

Cubic interpolation with C++ in Python.

![image](https://user-images.githubusercontent.com/5203275/211158357-3686465b-8363-4066-8535-1e2a648f5406.png)

## Introduction

This C++ header library features tools for piecewise cubic interpolation.

Currently, only 1D interpolation is supported, however, future released are planned to extend the library to higher dimensions.

The library features three kinds of different interpolation types:

- Monotone cubic interpolation
- Akima spline interpolation 
- Natural cubic spline interpolation

All classes are templatized and support the STL's vector types.

The accompanying python script in [cubinterpp ](cubinterpp ) compares the three
interpolation types.

## Usage

The following instructions show how to build and test the cubinterpp header
library in a python environment.

### Prerequisites

- C++ compiler, e.g. [gcc](https://gcc.gnu.org/)
- [cmake](https://cmake.org/): to use the provided cmake configuration
- [pybind11](https://github.com/pybind/pybind11): to compile the library header
  into a python module
- [mlpyqtgraph](https://github.com/swvanbuuren/mlpyqtgraph): to plot the
  example's results

## Building

To build the header library for usage in Python, it's recommended to use
[cmake](https://cmake.org/). An appropriate cmake configuration is provided in
the main [`CMakeLists.txt`](CMakeLists.txt). Prior to compilation, the required
external libraries are downloaded automatically using the cmake FetchContent
module. Prior to building, make sure cmake is installed and configured with a
C++ compiler like e.g. [gcc](https://gcc.gnu.org/).

Then build using:

```bash
cmake build
```

This should build the library in the build directory and automatically copy the
library file `cubic_spline.*.so` into the `cubinterpp` directory.

### Interpolating and plotting the results

A [python program](cubinterpp/main.py) is provided to compare the three
interpolation types. Data preparation and visualization is done in python with
[mlpyqtgraph](https://github.com/swvanbuuren/mlpyqtgraph).

In order to run the python program, it's recommended to [install
uv](https://docs.astral.sh/uv/getting-started/installation/#standalone-installer)
and issue:

```bash
uv run cubinterpp 
```

This should install all required python dependencies automatically and run the
python program that does the interpolation and plotting, resulting in the
comparison plot shown at the top of this document.

## License

An MIT style license applies for cubinterpp , see the [LICENSE](LICENSE) file for
more details.
