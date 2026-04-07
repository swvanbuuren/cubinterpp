

# Class cip::CubicInterpND&lt; T, 1, IM &gt;

**template &lt;typename T, IndexMethod IM&gt;**



[**ClassList**](annotated.md) **>** [**cip**](namespacecip.md) **>** [**CubicInterpND&lt; T, 1, IM &gt;**](classcip_1_1CubicInterpND_3_01T_00_011_00_01IM_01_4.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CubicInterpND**](#function-cubicinterpnd) (const Vector & \_x, const Vector & \_f) <br> |
|  void | [**build**](#function-build) (Vector f) <br> |
| virtual Vector | [**calc\_slopes**](#function-calc_slopes) (const Vector & x, const Mdspan1D & f) const = 0<br> |
|  T | [**eval**](#function-eval) (const T xi) const<br> |
|  Vector | [**evaln**](#function-evaln) (const Vector & xi) const<br> |
| virtual  | [**~CubicInterpND**](#function-cubicinterpnd) () <br> |




























## Public Functions Documentation




### function CubicInterpND 

```C++
inline cip::CubicInterpND< T, 1, IM >::CubicInterpND (
    const Vector & _x,
    const Vector & _f
) 
```




<hr>



### function build 

```C++
inline void cip::CubicInterpND< T, 1, IM >::build (
    Vector f
) 
```




<hr>



### function calc\_slopes 

```C++
virtual Vector cip::CubicInterpND< T, 1, IM >::calc_slopes (
    const Vector & x,
    const Mdspan1D & f
) const = 0
```




<hr>



### function eval 

```C++
inline T cip::CubicInterpND< T, 1, IM >::eval (
    const T xi
) const
```




<hr>



### function evaln 

```C++
inline Vector cip::CubicInterpND< T, 1, IM >::evaln (
    const Vector & xi
) const
```




<hr>



### function ~CubicInterpND 

```C++
inline virtual cip::CubicInterpND< T, 1, IM >::~CubicInterpND () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/old/cubic_interp_1d_spec.hpp`

