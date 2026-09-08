

# Class cip::CubicInterpND

**template &lt;typename T, std::size\_t N, IndexMethod IM&gt;**



[**ClassList**](annotated.md) **>** [**cip**](namespacecip.md) **>** [**CubicInterpND**](classcip_1_1CubicInterpND.md)










Inherited by the following classes: [cip::CubicInterp](classcip_1_1CubicInterp.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CubicInterpND**](#function-cubicinterpnd-12) (const [**Ff**](classcip_1_1VectorN.md) & \_f, const Args &... \_xi) <br> |
|   | [**CubicInterpND**](#function-cubicinterpnd-22) ([**CubicInterpND**](classcip_1_1CubicInterpND.md) &&) = delete<br> |
|  void | [**build**](#function-build) (const [**Ff**](classcip_1_1VectorN.md) & f, const Args &... \_xi) <br> |
| virtual Vector | [**calc\_slopes**](#function-calc_slopes) (const Vector & x, const Mdspan1D & f) const = 0<br> |
|  T | [**eval**](#function-eval) (const Args &... args) const<br> |
|  Vector | [**evaln**](#function-evaln) (const Vectors &... inputs) const<br> |
|  [**CubicInterpND**](classcip_1_1CubicInterpND.md) & | [**operator=**](#function-operator) ([**CubicInterpND**](classcip_1_1CubicInterpND.md) &&) = delete<br> |
| virtual  | [**~CubicInterpND**](#function-cubicinterpnd) () <br> |




























## Public Functions Documentation




### function CubicInterpND [1/2]

```C++
template<typename... Args>
inline cip::CubicInterpND::CubicInterpND (
    const Ff & _f,
    const Args &... _xi
) 
```




<hr>



### function CubicInterpND [2/2]

```C++
cip::CubicInterpND::CubicInterpND (
    CubicInterpND &&
) = delete
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



### function evaln 

```C++
template<typename... Vectors>
inline Vector cip::CubicInterpND::evaln (
    const Vectors &... inputs
) const
```




<hr>



### function operator= 

```C++
CubicInterpND & cip::CubicInterpND::operator= (
    CubicInterpND &&
) = delete
```




<hr>



### function ~CubicInterpND 

```C++
inline virtual cip::CubicInterpND::~CubicInterpND () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/cubic_interp.hpp`

