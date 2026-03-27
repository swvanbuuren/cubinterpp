

# Class cip::CubicInterpND

**template &lt;typename T, std::size\_t N&gt;**



[**ClassList**](annotated.md) **>** [**cip**](namespacecip.md) **>** [**CubicInterpND**](classcip_1_1CubicInterpND.md)










Inherited by the following classes: [cip::MakimaCubicInterp1D](classcip_1_1MakimaCubicInterp1D.md),  [cip::MakimaCubicInterp2D](classcip_1_1MakimaCubicInterp2D.md),  [cip::MonotonicCubicInterp1D](classcip_1_1MonotonicCubicInterp1D.md),  [cip::MonotonicCubicInterp2D](classcip_1_1MonotonicCubicInterp2D.md),  [cip::NaturalCubicInterp1D](classcip_1_1NaturalCubicInterp1D.md),  [cip::NaturalCubicInterp2D](classcip_1_1NaturalCubicInterp2D.md),  [cip::NaturalCubicInterp3D](classcip_1_1NaturalCubicInterp3D.md),  [cip::NaturalPeriodicCubicInterp1D](classcip_1_1NaturalPeriodicCubicInterp1D.md),  [cip::NaturalPeriodicCubicInterp2D](classcip_1_1NaturalPeriodicCubicInterp2D.md)
































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
The documentation for this class was generated from the following file `include/cubic_interp.hpp`

