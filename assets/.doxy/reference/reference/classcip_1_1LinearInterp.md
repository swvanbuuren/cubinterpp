

# Class cip::LinearInterp

**template &lt;typename T, std::size\_t N, IndexMethod IM&gt;**



[**ClassList**](annotated.md) **>** [**cip**](namespacecip.md) **>** [**LinearInterp**](classcip_1_1LinearInterp.md)








Inherits the following classes: [cip::LinearInterpND](classcip_1_1LinearInterpND.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**LinearInterp**](#function-linearinterp-14) (const Vector & x, const Vector & f) <br> |
|   | [**LinearInterp**](#function-linearinterp-24) (const Vector & x0, const Vector & x1, const [**VectorN**](classcip_1_1VectorN.md) & f) <br> |
|   | [**LinearInterp**](#function-linearinterp-34) (const Vector & x0, const Vector & x1, const Vector & x2, const [**VectorN**](classcip_1_1VectorN.md) & f) <br> |
|   | [**LinearInterp**](#function-linearinterp-44) (const Vector & x0, const Vector & x1, const Vector & x2, const Vector & x3, const [**VectorN**](classcip_1_1VectorN.md) & f) <br> |
|   | [**~LinearInterp**](#function-linearinterp) () = default<br> |


## Public Functions inherited from cip::LinearInterpND

See [cip::LinearInterpND](classcip_1_1LinearInterpND.md)

| Type | Name |
| ---: | :--- |
|   | [**LinearInterpND**](classcip_1_1LinearInterpND.md#function-linearinterpnd-12) (const [**F**](classcip_1_1VectorN.md) & \_f, const Args &... \_xi) <br> |
|   | [**LinearInterpND**](classcip_1_1LinearInterpND.md#function-linearinterpnd-22) ([**LinearInterpND**](classcip_1_1LinearInterpND.md) &&) = delete<br> |
|  T | [**eval**](classcip_1_1LinearInterpND.md#function-eval) (const Args &... args) const<br> |
|  Vector | [**evaln**](classcip_1_1LinearInterpND.md#function-evaln) (const Vectors &... inputs) const<br> |
|  [**LinearInterpND**](classcip_1_1LinearInterpND.md) & | [**operator=**](classcip_1_1LinearInterpND.md#function-operator) ([**LinearInterpND**](classcip_1_1LinearInterpND.md) &&) = delete<br> |
|   | [**~LinearInterpND**](classcip_1_1LinearInterpND.md#function-linearinterpnd) () <br> |






















































## Public Functions Documentation




### function LinearInterp [1/4]

```C++
inline cip::LinearInterp::LinearInterp (
    const Vector & x,
    const Vector & f
) 
```




<hr>



### function LinearInterp [2/4]

```C++
inline cip::LinearInterp::LinearInterp (
    const Vector & x0,
    const Vector & x1,
    const VectorN & f
) 
```




<hr>



### function LinearInterp [3/4]

```C++
inline cip::LinearInterp::LinearInterp (
    const Vector & x0,
    const Vector & x1,
    const Vector & x2,
    const VectorN & f
) 
```




<hr>



### function LinearInterp [4/4]

```C++
inline cip::LinearInterp::LinearInterp (
    const Vector & x0,
    const Vector & x1,
    const Vector & x2,
    const Vector & x3,
    const VectorN & f
) 
```




<hr>



### function ~LinearInterp 

```C++
cip::LinearInterp::~LinearInterp () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/linear_interp_impl.hpp`

