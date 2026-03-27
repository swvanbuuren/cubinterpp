

# Class cip::MonotonicCubicInterp2D

**template &lt;typename T, std::size\_t N&gt;**



[**ClassList**](annotated.md) **>** [**cip**](namespacecip.md) **>** [**MonotonicCubicInterp2D**](classcip_1_1MonotonicCubicInterp2D.md)








Inherits the following classes: [cip::CubicInterpND](classcip_1_1CubicInterpND.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**MonotonicCubicInterp2D**](#function-monotoniccubicinterp2d) (const Vector & \_x, const Vector & \_y, const [**VectorN**](classcip_1_1VectorN.md) & \_f) <br> |
| virtual Vector | [**calc\_slopes**](#function-calc_slopes) (const Vector & x, const Mdspan1D & f) override const<br> |
|   | [**~MonotonicCubicInterp2D**](#function-monotoniccubicinterp2d) () <br> |


## Public Functions inherited from cip::CubicInterpND

See [cip::CubicInterpND](classcip_1_1CubicInterpND.md)

| Type | Name |
| ---: | :--- |
|   | [**CubicInterpND**](classcip_1_1CubicInterpND.md#function-cubicinterpnd) (const [**Ff**](classcip_1_1VectorN.md) & \_f, const Args &... \_xi) <br> |
|  void | [**build**](classcip_1_1CubicInterpND.md#function-build) (const [**Ff**](classcip_1_1VectorN.md) & f, const Args &... \_xi) <br> |
| virtual Vector | [**calc\_slopes**](classcip_1_1CubicInterpND.md#function-calc_slopes) (const Vector & x, const Mdspan1D & f) const = 0<br> |
|  T | [**eval**](classcip_1_1CubicInterpND.md#function-eval) (const Args &... args) const<br> |
| virtual  | [**~CubicInterpND**](classcip_1_1CubicInterpND.md#function-cubicinterpnd) () <br> |






















































## Public Functions Documentation




### function MonotonicCubicInterp2D 

```C++
inline cip::MonotonicCubicInterp2D::MonotonicCubicInterp2D (
    const Vector & _x,
    const Vector & _y,
    const VectorN & _f
) 
```




<hr>



### function calc\_slopes 

```C++
inline virtual Vector cip::MonotonicCubicInterp2D::calc_slopes (
    const Vector & x,
    const Mdspan1D & f
) override const
```



Implements [*cip::CubicInterpND::calc\_slopes*](classcip_1_1CubicInterpND.md#function-calc_slopes)


<hr>



### function ~MonotonicCubicInterp2D 

```C++
inline cip::MonotonicCubicInterp2D::~MonotonicCubicInterp2D () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/cubic_interp_2d.hpp`

