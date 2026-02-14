

# Class cip::CubicInterpND

**template &lt;typename T, std::size\_t N&gt;**



[**ClassList**](annotated.md) **>** [**cip**](namespacecip.md) **>** [**CubicInterpND**](classcip_1_1CubicInterpND.md)










Inherited by the following classes: [cip::AkimaSpline1D](classcip_1_1AkimaSpline1D.md),  [cip::AkimaSpline2D](classcip_1_1AkimaSpline2D.md),  [cip::MonotonicSpline1D](classcip_1_1MonotonicSpline1D.md),  [cip::MonotonicSpline2D](classcip_1_1MonotonicSpline2D.md),  [cip::NaturalPeriodicSpline1D](classcip_1_1NaturalPeriodicSpline1D.md),  [cip::NaturalPeriodicSpline2D](classcip_1_1NaturalPeriodicSpline2D.md),  [cip::NaturalSpline1D](classcip_1_1NaturalSpline1D.md),  [cip::NaturalSpline2D](classcip_1_1NaturalSpline2D.md),  [cip::NaturalSpline3D](classcip_1_1NaturalSpline3D.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CubicInterpND**](#function-cubicinterpnd) (const [**Ff**](classcip_1_1VectorN.md) & \_f, const Args &... \_xi) <br> |
|  void | [**build**](#function-build) (const [**Ff**](classcip_1_1VectorN.md) & f, const Args &... \_xi) <br> |
| virtual Vector | [**calc\_slopes**](#function-calc_slopes) (const Vector & x, const Mdspan1D & f) const = 0<br> |
|  T | [**eval**](#function-eval) (const Args &... args) const<br> |
| virtual  | [**~CubicInterpND**](#function-cubicinterpnd) () <br> |




























## Public Functions Documentation




### function CubicInterpND 

```C++
template<typename... Args>
inline cip::CubicInterpND::CubicInterpND (
    const Ff & _f,
    const Args &... _xi
) 
```




<hr>



### function build 

```C++
template<typename... Args>
inline void cip::CubicInterpND::build (
    const Ff & f,
    const Args &... _xi
) 
```




<hr>



### function calc\_slopes 

```C++
virtual Vector cip::CubicInterpND::calc_slopes (
    const Vector & x,
    const Mdspan1D & f
) const = 0
```




<hr>



### function eval 

```C++
template<typename... Args>
inline T cip::CubicInterpND::eval (
    const Args &... args
) const
```




<hr>



### function ~CubicInterpND 

```C++
inline virtual cip::CubicInterpND::~CubicInterpND () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/cubic_spline.hpp`

