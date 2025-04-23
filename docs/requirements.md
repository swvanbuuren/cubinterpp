# Requirements

The following requirements need to be met to build and properly use cubinterpp.

## Installation prerequisites 

- C++ compiler, capable of compiling C++20 or later, e.g.
  [gcc](https://gcc.gnu.org/)
- [cmake](https://cmake.org/): to use the provided cmake configuration
- [pybind11](https://github.com/pybind/pybind11): to compile the library header
  into a python module
- [mlpyqtgraph](https://github.com/swvanbuuren/mlpyqtgraph): to plot the
  example's results

## Data requirements

`cubinterpp` is designed to only handle rectangular grid data with strictly
ascending coordinates. To this end, coordinate data is only required in one
dimensional form for each coordinate direction. Only the actual data itself
needs to be supplied in the actual dimensions. The user is responsible to assure
that the supplied data size of each dimension fits to the coordinate data.
