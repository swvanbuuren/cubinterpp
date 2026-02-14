

# Class cip::CubicInterpND&lt; T, 1 &gt;

**template &lt;typename T&gt;**



[**ClassList**](annotated.md) **>** [**cip**](namespacecip.md) **>** [**CubicInterpND&lt; T, 1 &gt;**](classcip_1_1CubicInterpND_3_01T_00_011_01_4.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CubicInterpND**](#function-cubicinterpnd) (const Vector & \_x, const Vector & \_f) <br> |
|  void | [**build**](#function-build) (Vector f) <br> |
| virtual Vector | [**calc\_slopes**](#function-calc_slopes) (const Vector & x, const Vector & f) const = 0<br> |
|  T | [**eval**](#function-eval) (const T xi) const<br> |
|  Vector | [**evaln**](#function-evaln) (const Vector & xi) const<br> |
| virtual  | [**~CubicInterpND**](#function-cubicinterpnd) () <br> |




























## Public Functions Documentation




### function CubicInterpND 

```C++
inline cip::CubicInterpND< T, 1 >::CubicInterpND (
    const Vector & _x,
    const Vector & _f
) 
```




<hr>



### function build 

```C++
inline void cip::CubicInterpND< T, 1 >::build (
    Vector f
) 
```




<hr>



### function calc\_slopes 

```C++
virtual Vector cip::CubicInterpND< T, 1 >::calc_slopes (
    const Vector & x,
    const Vector & f
) const = 0
```




<hr>



### function eval 

```C++
inline T cip::CubicInterpND< T, 1 >::eval (
    const T xi
) const
```




<hr>



### function evaln 

```C++
inline Vector cip::CubicInterpND< T, 1 >::evaln (
    const Vector & xi
) const
```




<hr>



### function ~CubicInterpND 

```C++
inline virtual cip::CubicInterpND< T, 1 >::~CubicInterpND () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/cubic_spline.hpp`

