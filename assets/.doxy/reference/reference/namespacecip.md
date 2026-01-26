

# Namespace cip



[**Namespace List**](namespaces.md) **>** [**cip**](namespacecip.md)




















## Classes

| Type | Name |
| ---: | :--- |
| class | [**AkimaSpline1D**](classcip_1_1AkimaSpline1D.md) &lt;typename T, N&gt;<br> |
| class | [**AkimaSpline2D**](classcip_1_1AkimaSpline2D.md) &lt;typename T, N&gt;<br> |
| class | [**CubicCellND**](classcip_1_1CubicCellND.md) &lt;typename T, N&gt;<br> |
| class | [**CubicCellND&lt; T, 1 &gt;**](classcip_1_1CubicCellND_3_01T_00_011_01_4.md) &lt;typename T&gt;<br> |
| class | [**CubicInterpND**](classcip_1_1CubicInterpND.md) &lt;typename T, N&gt;<br> |
| class | [**CubicInterpND&lt; T, 1 &gt;**](classcip_1_1CubicInterpND_3_01T_00_011_01_4.md) &lt;typename T&gt;<br> |
| class | [**Indexer**](classcip_1_1Indexer.md) &lt;typename T&gt;<br> |
| class | [**LinearCellND**](classcip_1_1LinearCellND.md) &lt;typename T, N&gt;<br> |
| class | [**LinearCellND&lt; T, 1 &gt;**](classcip_1_1LinearCellND_3_01T_00_011_01_4.md) &lt;typename T&gt;<br> |
| class | [**LinearInterp1D**](classcip_1_1LinearInterp1D.md) &lt;typename T&gt;<br> |
| class | [**LinearInterp2D**](classcip_1_1LinearInterp2D.md) &lt;typename T&gt;<br> |
| class | [**LinearInterp3D**](classcip_1_1LinearInterp3D.md) &lt;typename T&gt;<br> |
| class | [**LinearInterp4D**](classcip_1_1LinearInterp4D.md) &lt;typename T&gt;<br> |
| class | [**LinearInterpND**](classcip_1_1LinearInterpND.md) &lt;typename T, N&gt;<br> |
| class | [**LinearInterpND&lt; T, 1 &gt;**](classcip_1_1LinearInterpND_3_01T_00_011_01_4.md) &lt;typename T&gt;<br> |
| class | [**MonotonicSpline1D**](classcip_1_1MonotonicSpline1D.md) &lt;typename T, N&gt;<br> |
| class | [**MonotonicSpline2D**](classcip_1_1MonotonicSpline2D.md) &lt;typename T, N&gt;<br> |
| class | [**NaturalPeriodicSpline1D**](classcip_1_1NaturalPeriodicSpline1D.md) &lt;typename T, N, BC&gt;<br> |
| class | [**NaturalPeriodicSpline2D**](classcip_1_1NaturalPeriodicSpline2D.md) &lt;typename T, BC, N&gt;<br> |
| class | [**NaturalSpline1D**](classcip_1_1NaturalSpline1D.md) &lt;typename T, N, BC&gt;<br> |
| class | [**NaturalSpline2D**](classcip_1_1NaturalSpline2D.md) &lt;typename T, BC, N&gt;<br> |
| class | [**NaturalSpline3D**](classcip_1_1NaturalSpline3D.md) &lt;typename T, BC, N&gt;<br> |
| class | [**VectorN**](classcip_1_1VectorN.md) &lt;typename T, N&gt;<br> |
| struct | [**setNaturalSplineBoundaryCondition**](structcip_1_1setNaturalSplineBoundaryCondition.md) &lt;BC, typename T, typename Tx, typename Tf&gt;<br> |
| struct | [**setNaturalSplineBoundaryCondition&lt; BoundaryConditionType::Clamped, T, Tx, Tf &gt;**](structcip_1_1setNaturalSplineBoundaryCondition_3_01BoundaryConditionType_1_1Clamped_00_01T_00_01Tx_00_01Tf_01_4.md) &lt;typename T, typename Tx, typename Tf&gt;<br> |
| struct | [**setNaturalSplineBoundaryCondition&lt; BoundaryConditionType::Natural, T, Tx, Tf &gt;**](structcip_1_1setNaturalSplineBoundaryCondition_3_01BoundaryConditionType_1_1Natural_00_01T_00_01Tx_00_01Tf_01_4.md) &lt;typename T, typename Tx, typename Tf&gt;<br> |
| struct | [**setNaturalSplineBoundaryCondition&lt; BoundaryConditionType::NotAKnot, T, Tx, Tf &gt;**](structcip_1_1setNaturalSplineBoundaryCondition_3_01BoundaryConditionType_1_1NotAKnot_00_01T_00_01Tx_00_01Tf_01_4.md) &lt;typename T, typename Tx, typename Tf&gt;<br> |
| struct | [**setNaturalSplineBoundaryCondition&lt; BoundaryConditionType::Periodic, T, Tx, Tf &gt;**](structcip_1_1setNaturalSplineBoundaryCondition_3_01BoundaryConditionType_1_1Periodic_00_01T_00_01Tx_00_01Tf_01_4.md) &lt;typename T, typename Tx, typename Tf&gt;<br> |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**BoundaryConditionType**](#enum-boundaryconditiontype)  <br> |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  constexpr std::size\_t | [**SIZE\_T\_ZERO**](#variable-size_t_zero)   = `0`<br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  std::vector&lt; T &gt; | [**akima\_slopes**](#function-akima_slopes) (const Tx x, const Tf f) <br> |
|  constexpr std::size\_t | [**binomial**](#function-binomial) (std::size\_t n, std::size\_t k) noexcept<br> |
|  T | [**binomial\_power\_coefficient**](#function-binomial_power_coefficient) (const T y, const int n, const int k) <br> |
|  constexpr auto | [**build\_coordinate\_indices**](#function-build_coordinate_indices) (const Tuple & t) <br> |
|  constexpr auto | [**build\_coordinate\_indices\_impl**](#function-build_coordinate_indices_impl) (const Tuple & t, Accum acc) <br> |
|  void | [**call\_move\_into\_submdspan**](#function-call_move_into_submdspan) (FType & F, SlopeType && slopes, const CoordTuple & coord, const DerivTuple & deriv) <br> |
|  auto | [**call\_submdspan\_1d**](#function-call_submdspan_1d) (FType & F, const CoordTuple & coord, const DerivTuple & deriv) <br> |
|  void | [**compute\_mixed\_derivatives**](#function-compute_mixed_derivatives) (FType & F, XiArray const & xi, CalcSlopesFunctor calcSlopes) <br> |
|  void | [**compute\_mixed\_derivatives\_impl**](#function-compute_mixed_derivatives_impl) (FType & F, XiArray const & xi, const DerivTuple & currentDeriv, CalcSlopesFunctor calcSlopes, std::size\_t start=0) <br> |
|  void | [**cyclic\_thomas\_algorithm**](#function-cyclic_thomas_algorithm) (const std::vector&lt; T &gt; & a, const std::vector&lt; T &gt; & b, const std::vector&lt; T &gt; & c, std::vector&lt; T &gt; & d, const std::size\_t exclude\_rows=0) <br> |
|  constexpr int | [**factorial**](#function-factorial) (int n) <br> |
|  constexpr void | [**for\_each\_dimension**](#function-for_each_dimension) (const Tuple & tup, Func && func) <br> |
|  constexpr void | [**for\_each\_dimension\_impl**](#function-for_each_dimension_impl) (const Tuple & tup, Func && func, std::index\_sequence&lt; Is... &gt;) <br> |
|  void | [**iterate\_over\_indices**](#function-iterate_over_indices) (Func && func, XiArray const & xi, Indices... indices) <br> |
|  constexpr auto | [**make\_zero\_tuple**](#function-make_zero_tuple) () <br> |
|  std::vector&lt; T &gt; | [**monotonic\_slopes**](#function-monotonic_slopes) (const Tx x, const Tf f) <br> |
|  std::vector&lt; T &gt; | [**natural\_spline\_slopes**](#function-natural_spline_slopes) (const Tx x, const Tf f) <br> |
|  constexpr std::size\_t | [**power**](#function-power) () <br> |
|  void | [**thomas\_algorithm**](#function-thomas_algorithm) (const std::vector&lt; T &gt; & a, const std::vector&lt; T &gt; & b, std::vector&lt; T &gt; & c, std::vector&lt; T &gt; & d) <br> |
|  constexpr auto | [**update\_tuple\_element**](#function-update_tuple_element) (const Tuple & tup, T new\_value) <br> |




























## Public Types Documentation




### enum BoundaryConditionType 

```C++
enum cip::BoundaryConditionType {
    Natural,
    Clamped,
    NotAKnot,
    Periodic
};
```




<hr>
## Public Attributes Documentation




### variable SIZE\_T\_ZERO 

```C++
constexpr std::size_t cip::SIZE_T_ZERO;
```




<hr>
## Public Functions Documentation




### function akima\_slopes 

```C++
template<typename T, typename Tx, typename Tf>
std::vector< T > cip::akima_slopes (
    const Tx x,
    const Tf f
) 
```




<hr>



### function binomial 

```C++
inline constexpr std::size_t cip::binomial (
    std::size_t n,
    std::size_t k
) noexcept
```




<hr>



### function binomial\_power\_coefficient 

```C++
template<typename T>
T cip::binomial_power_coefficient (
    const T y,
    const int n,
    const int k
) 
```




<hr>



### function build\_coordinate\_indices 

```C++
template<std::size_t N, std::size_t D, typename Tuple>
constexpr auto cip::build_coordinate_indices (
    const Tuple & t
) 
```




<hr>



### function build\_coordinate\_indices\_impl 

```C++
template<std::size_t I, std::size_t N, std::size_t D, typename Tuple, typename Accum>
constexpr auto cip::build_coordinate_indices_impl (
    const Tuple & t,
    Accum acc
) 
```




<hr>



### function call\_move\_into\_submdspan 

```C++
template<typename FType, typename SlopeType, typename CoordTuple, typename DerivTuple>
void cip::call_move_into_submdspan (
    FType & F,
    SlopeType && slopes,
    const CoordTuple & coord,
    const DerivTuple & deriv
) 
```




<hr>



### function call\_submdspan\_1d 

```C++
template<typename FType, typename CoordTuple, typename DerivTuple>
auto cip::call_submdspan_1d (
    FType & F,
    const CoordTuple & coord,
    const DerivTuple & deriv
) 
```




<hr>



### function compute\_mixed\_derivatives 

```C++
template<std::size_t N, typename FType, typename XiArray, typename CalcSlopesFunctor>
void cip::compute_mixed_derivatives (
    FType & F,
    XiArray const & xi,
    CalcSlopesFunctor calcSlopes
) 
```




<hr>



### function compute\_mixed\_derivatives\_impl 

```C++
template<std::size_t N, typename FType, typename XiArray, typename DerivTuple, typename CalcSlopesFunctor>
void cip::compute_mixed_derivatives_impl (
    FType & F,
    XiArray const & xi,
    const DerivTuple & currentDeriv,
    CalcSlopesFunctor calcSlopes,
    std::size_t start=0
) 
```




<hr>



### function cyclic\_thomas\_algorithm 

```C++
template<typename T>
void cip::cyclic_thomas_algorithm (
    const std::vector< T > & a,
    const std::vector< T > & b,
    const std::vector< T > & c,
    std::vector< T > & d,
    const std::size_t exclude_rows=0
) 
```




<hr>



### function factorial 

```C++
inline constexpr int cip::factorial (
    int n
) 
```




<hr>



### function for\_each\_dimension 

```C++
template<std::size_t N, typename Tuple, typename Func>
constexpr void cip::for_each_dimension (
    const Tuple & tup,
    Func && func
) 
```




<hr>



### function for\_each\_dimension\_impl 

```C++
template<std::size_t N, typename Tuple, typename Func, std::size_t... Is>
constexpr void cip::for_each_dimension_impl (
    const Tuple & tup,
    Func && func,
    std::index_sequence< Is... >
) 
```




<hr>



### function iterate\_over\_indices 

```C++
template<std::size_t N, std::size_t D, std::size_t current, typename Func, typename XiArray, typename... Indices>
void cip::iterate_over_indices (
    Func && func,
    XiArray const & xi,
    Indices... indices
) 
```




<hr>



### function make\_zero\_tuple 

```C++
template<std::size_t N>
constexpr auto cip::make_zero_tuple () 
```




<hr>



### function monotonic\_slopes 

```C++
template<typename T, typename Tx, typename Tf>
std::vector< T > cip::monotonic_slopes (
    const Tx x,
    const Tf f
) 
```




<hr>



### function natural\_spline\_slopes 

```C++
template<typename T, BoundaryConditionType BC, typename Tx, typename Tf>
std::vector< T > cip::natural_spline_slopes (
    const Tx x,
    const Tf f
) 
```




<hr>



### function power 

```C++
template<std::size_t Base, std::size_t Exp>
constexpr std::size_t cip::power () 
```




<hr>



### function thomas\_algorithm 

```C++
template<typename T>
void cip::thomas_algorithm (
    const std::vector< T > & a,
    const std::vector< T > & b,
    std::vector< T > & c,
    std::vector< T > & d
) 
```




<hr>



### function update\_tuple\_element 

```C++
template<std::size_t D, typename Tuple, typename T>
constexpr auto cip::update_tuple_element (
    const Tuple & tup,
    T new_value
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/cubic_spline.hpp`

