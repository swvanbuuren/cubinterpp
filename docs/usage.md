# Usage instructions

The following instructions show how to build and test the cubinterpp header
library in a python environment.

## Prerequisites

Refer to [installation prerequisites](requirements.md#installation-prerequisites).

## Single header library

The easiest way to use cubinterpp in your project is to use the single header
library.

Starting with [release
v0.8.0](https://github.com/swvanbuuren/cubinterpp/releases/tag/v0.8.0) each
release comes with the single header library as asset. Alternatively, you can
build the single header library yourself with (assuming `python3` is already
installed):

```console
python3 create_single_header.py \
          --entry include/cubinterpp.hpp \
          --output ./build/cubinterpp_header.hpp \
          --license LICENSE
```

Since [mdspan](https://github.com/kokkos/mdspan) is not yet supported in the
standard library you also need to get its corresponding single header library
from [the mdspan
repository](https://raw.githubusercontent.com/kokkos/mdspan/refs/heads/single-header/mdspan.hpp).

After putting both `cubinterpp_header.hpp` and `mdspan.hpp` into you `include`
directory, you can use cubinterpp in your project by just adding it to the
includes in your codebase:

```cpp
#include "cubinterpp_header.hpp"
```

## Build from source

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

By default, the library offers both linear and cubic spline interpolation classes
up to three dimensions with `std::vector` input types. If you'd like to implement
higher dimensions, it is recommended to inherit from the respective
`N`-dimensional base class.

### Linear interpolation

For linear interpolation beyond three dimensions, inherit from
`LinearInterpND<T, N>`. For example, for four-dimensional linear interpolation:

```cpp
#include "linear_interp.hpp"

template <typename T>
class LinearInterp4D : public LinearInterpND<T, 4> {
    using Vector = std::vector<T>;
    using Vector4 = cip::VectorN<T, 4>;
public:
    explicit LinearInterp4D(const Vector &x, const Vector &y,
                             const Vector &z, const Vector &w, const Vector4 &f)
    : LinearInterpND<T, 4>(f, x, y, z, w)
    {}

    ~LinearInterp4D() { }
};
```

To use the direct-formula cell indexer (see [Indexing method](#indexing-method)
below), pass `cip::IndexMethod::Cell` as the third template argument:

```cpp
class LinearInterp4D : public LinearInterpND<T, 4, cip::IndexMethod::Cell> { ... };
```

### Cubic spline interpolation

For cubic spline interpolation in 1, 2, or 3 dimensions, the `CubicInterp` class
template in `cubic_interp_impl.hpp` can be used directly via type aliases such as
`MonotonicCubicInterp1D`, `NaturalCubicInterp2D`, `NaturalCubicInterp3D`, etc.

For higher dimensions, inherit from `CubicInterpND<T, N>` and implement the pure
virtual `calc_slopes` method. `SlopePolicy` from `cubic_interp_impl.hpp` can be
used to reuse the built-in slope algorithms. For example, for four-dimensional
monotone cubic interpolation:

```cpp
#include "cubic_interp.hpp"
#include "cubic_interp_impl.hpp"

template <typename T>
class MonotonicCubicInterp4D : public cip::CubicInterpND<T, 4> {
    using Vector = std::vector<T>;
    using Vector4 = cip::VectorN<T, 4>;
    using Mdspan1D = std::mdspan<T, std::dextents<std::size_t, 1>, std::layout_stride>;
public:
    explicit MonotonicCubicInterp4D(const Vector &x0, const Vector &x1,
                                     const Vector &x2, const Vector &x3,
                                     const Vector4 &f)
    : cip::CubicInterpND<T, 4>(f, x0, x1, x2, x3)
    {
        this->build(f, x0, x1, x2, x3);
    }

    ~MonotonicCubicInterp4D() { }

    Vector calc_slopes(const Vector& x, const Mdspan1D& f) const override {
        return cip::SlopePolicy<cip::SlopeMethod::Monotonic>::template calc<T>(x, f);
    }
};
```

To use the direct-formula cell indexer, pass `cip::IndexMethod::Cell` as the
third template argument of the base class:

```cpp
class MonotonicCubicInterp4D : public cip::CubicInterpND<T, 4, cip::IndexMethod::Cell> { ... };
```

!!! note
    Note the counter-intuitive order of the constructor arguments in `LinearInterpND`
    and `CubicInterpND`, due to the requirement that a parameter pack always needs to
    come last. This can be corrected in the inheriting class's constructor. It is also
    possible to use different input types, which might differ per application.

## Indexing method

All interpolation classes select the cell index via a compile-time template
parameter of type `cip::IndexMethod` (defined in `utils.hpp`).  Two strategies
are available:

| Value | Strategy | When to use |
|---|---|---|
| `cip::IndexMethod::Sorted` *(default)* | Binary search (`std::upper_bound`) | Non-uniform or arbitrary grid spacing |
| `cip::IndexMethod::Cell` | Direct formula $\lfloor(x - x_0) / \Delta x\rfloor$ | Uniformly spaced grids only — faster |

For the ready-to-use type aliases (`MonotonicCubicInterp1D`,
`NaturalCubicInterp2D`, `LinearInterp1D`, etc.) the indexing method is the last
template parameter and defaults to `cip::IndexMethod::Sorted`:

```cpp
// default — works with any grid spacing
cip::NaturalCubicInterp1D<double> spline(x, f);

// explicit sorted indexing
cip::NaturalCubicInterp1D<double,
                           cip::BoundaryConditionType::Natural,
                           cip::IndexMethod::Sorted> spline(x, f);

// cell indexing — faster, but only valid for uniform grids
cip::NaturalCubicInterp1D<double,
                           cip::BoundaryConditionType::Natural,
                           cip::IndexMethod::Cell> spline(x, f);
```

!!! warning "Cell indexing requires evenly spaced data"
    `cip::IndexMethod::Cell` computes the cell index directly using the formula
    $\lfloor(x - x_0) / \Delta x\rfloor$, where $\Delta x = (x_\text{back} - x_\text{front}) / (n - 1)$.  
    This is **only correct when all input coordinate vectors are uniformly
    spaced**.  Passing non-uniform grids with `IndexMethod::Cell` will silently
    produce wrong results — no runtime check is performed.
