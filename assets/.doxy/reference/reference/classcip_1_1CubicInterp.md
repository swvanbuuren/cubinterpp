

# Class cip::CubicInterp

**template &lt;typename T, typename [**SlopePolicy**](structcip_1_1SlopePolicy.md), std::size\_t N, IndexMethod IM&gt;**



[**ClassList**](annotated.md) **>** [**cip**](namespacecip.md) **>** [**CubicInterp**](classcip_1_1CubicInterp.md)








Inherits the following classes: [cip::CubicInterpND](classcip_1_1CubicInterpND.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CubicInterp**](#function-cubicinterp-13) (const Vector & x, const Vector & f) <br> |
|   | [**CubicInterp**](#function-cubicinterp-23) (const Vector & x0, const Vector & x1, const [**VectorN**](classcip_1_1VectorN.md) & f) <br> |
|   | [**CubicInterp**](#function-cubicinterp-33) (const Vector & x0, const Vector & x1, const Vector & x2, const [**VectorN**](classcip_1_1VectorN.md) & f) <br> |
| virtual Vector | [**calc\_slopes**](#function-calc_slopes) (const Vector & x, const Mdspan1D & f) override const<br> |
|   | [**~CubicInterp**](#function-cubicinterp) () = default<br> |


## Public Functions inherited from cip::CubicInterpND

See [cip::CubicInterpND](classcip_1_1CubicInterpND.md)

| Type | Name |
| ---: | :--- |
|   | [**CubicInterpND**](classcip_1_1CubicInterpND.md#function-cubicinterpnd-12) (const [**Ff**](classcip_1_1VectorN.md) & \_f, const Args &... \_xi) <br> |
|   | [**CubicInterpND**](classcip_1_1CubicInterpND.md#function-cubicinterpnd-22) ([**CubicInterpND**](classcip_1_1CubicInterpND.md) &&) = delete<br> |
|  void | [**build**](classcip_1_1CubicInterpND.md#function-build) (const [**Ff**](classcip_1_1VectorN.md) & f, const Args &... \_xi) <br> |
| virtual Vector | [**calc\_slopes**](classcip_1_1CubicInterpND.md#function-calc_slopes) (const Vector & x, const Mdspan1D & f) const = 0<br> |
|  T | [**eval**](classcip_1_1CubicInterpND.md#function-eval) (const Args &... args) const<br> |
|  Vector | [**evaln**](classcip_1_1CubicInterpND.md#function-evaln) (const Vectors &... inputs) const<br> |
|  [**CubicInterpND**](classcip_1_1CubicInterpND.md) & | [**operator=**](classcip_1_1CubicInterpND.md#function-operator) ([**CubicInterpND**](classcip_1_1CubicInterpND.md) &&) = delete<br> |
| virtual  | [**~CubicInterpND**](classcip_1_1CubicInterpND.md#function-cubicinterpnd) () <br> |






















































## Public Functions Documentation




### function CubicInterp [1/3]

```C++
inline cip::CubicInterp::CubicInterp (
    const Vector & x,
    const Vector & f
) 
```




<hr>



### function CubicInterp [2/3]

```C++
inline cip::CubicInterp::CubicInterp (
    const Vector & x0,
    const Vector & x1,
    const VectorN & f
) 
```




<hr>



### function CubicInterp [3/3]

```C++
inline cip::CubicInterp::CubicInterp (
    const Vector & x0,
    const Vector & x1,
    const Vector & x2,
    const VectorN & f
) 
```




<hr>



### function calc\_slopes 

```C++
inline virtual Vector cip::CubicInterp::calc_slopes (
    const Vector & x,
    const Mdspan1D & f
) override const
```



Implements [*cip::CubicInterpND::calc\_slopes*](classcip_1_1CubicInterpND.md#function-calc_slopes)


<hr>



### function ~CubicInterp 

```C++
cip::CubicInterp::~CubicInterp () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/cubic_interp_impl.hpp`

