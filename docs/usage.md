# Usage instructions

The following instructions show how to build and test the cubinterpp header
library in a python environment.

## Prerequisites

Refer to [installation prerequisites](requirements.md#installation-prerequisites).

## Building

To build the header library for usage in Python, it's recommended to use
[cmake](https://cmake.org/). An appropriate cmake configuration is provided in
the main
[`CMakeLists.txt`](https://github.com/swvanbuuren/cubinterpp/blob/master/CMakeLists.txt).
Prior to compilation, the required external libraries are downloaded
automatically using the cmake FetchContent module. Prior to building, make sure
`cmake` is installed and configured with a C++ compiler like e.g.
[gcc](https://gcc.gnu.org/). In order to create the python module, the
development python library is also required.

In order to do so on a Debian based system, install `cmake`, `gcc`, `g++` and
`python3.11-dev` (change the python version depending on your configuration):

```bash
sudo apt install cmake gcc g++ python3.11-dev
```

Set the appropriate environment variables (it's recommended to add these lines
to e.g. your `.bashrc`):

```bash
export CC=/usr/bin/gcc
export CXX=/usr/bin/g++
```

Then create the build directory, configure and build using:

```bash
mkdir build
cmake ..
make
```

This should build and automatically copy the library file `cubic_spline.*.so`
into the `cubinterpp` directory.

## Testing

This library comes with severals
[tests](https://github.com/swvanbuuren/cubinterpp/tree/master/tests). To run all
tests, first build and then run (while remaining in the `build` directory):

```bash
ctest -V
```

## Interpolating and plotting the results

A [python
program](https://github.com/swvanbuuren/cubinterpp/blob/master/cubinterpp/main.py)
is provided to compare the three interpolation types. Data preparation and
visualization is done in python with
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

## Higher interpolation dimensions

By default, the library offers linear interpolation classes up to three
dimensions with `std::vector` input types. If you'd like to implement higher
dimensions, it's recommended to inherit from the `N-dimensional` interpolation
class for a given dimension. For example, for three dimensional linear
interpolation this could look like:

```cpp
#include "linear_interp.hpp"

template <typename T>
class LinearInterp3D : public LinearInterpND<T, 3> {
    using Vector = std::vector<T>;
    using Vector3 = cip::VectorN<T, 3>;
public:
    explicit LinearInterp3D(const Vector &x, const Vector &y, const Vector &z, const Vector3 &f)
    : LinearInterpND<T, 3>(f, x, y, z)
    {}

    ~LinearInterp3D() { }
};
```

Note the counter-intuitive order of the constructor argument in
`LinearInterpND`, due to the requirement that a parameter pack always needs to
come last. This can be corrected in the inheriting classes constructor. Here,
it's also possible to use different input types, which might differ per
application.
