

# Class cip::MakimaCubicInterp1D

**template &lt;typename T, std::size\_t N&gt;**



[**ClassList**](annotated.md) **>** [**cip**](namespacecip.md) **>** [**MakimaCubicInterp1D**](classcip_1_1MakimaCubicInterp1D.md)








Inherits the following classes: [cip::CubicInterpND](classcip_1_1CubicInterpND.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**MakimaCubicInterp1D**](#function-makimacubicinterp1d) (const Vector & x, const Vector & f) <br> |
|  Vector | [**calc\_slopes**](#function-calc_slopes) (const Vector & x, const Vector & f) override const<br> |
|   | [**~MakimaCubicInterp1D**](#function-makimacubicinterp1d) () <br> |


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




### function MakimaCubicInterp1D 

```C++
inline cip::MakimaCubicInterp1D::MakimaCubicInterp1D (
    const Vector & x,
    const Vector & f
) 
```




<hr>



### function calc\_slopes 

```C++
inline Vector cip::MakimaCubicInterp1D::calc_slopes (
    const Vector & x,
    const Vector & f
) override const
```




<hr>



### function ~MakimaCubicInterp1D 

```C++
inline cip::MakimaCubicInterp1D::~MakimaCubicInterp1D () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/cubic_interp_1d.hpp`

