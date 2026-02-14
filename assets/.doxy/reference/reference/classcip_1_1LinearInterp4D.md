

# Class cip::LinearInterp4D

**template &lt;typename T&gt;**



[**ClassList**](annotated.md) **>** [**cip**](namespacecip.md) **>** [**LinearInterp4D**](classcip_1_1LinearInterp4D.md)








Inherits the following classes: [cip::LinearInterpND](classcip_1_1LinearInterpND.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**LinearInterp4D**](#function-linearinterp4d) (const Vector & x, const Vector & y, const Vector & z, const Vector & w, const [**Vector4**](classcip_1_1VectorN.md) & f) <br> |
|   | [**~LinearInterp4D**](#function-linearinterp4d) () <br> |


## Public Functions inherited from cip::LinearInterpND

See [cip::LinearInterpND](classcip_1_1LinearInterpND.md)

| Type | Name |
| ---: | :--- |
|   | [**LinearInterpND**](classcip_1_1LinearInterpND.md#function-linearinterpnd) (const [**F**](classcip_1_1VectorN.md) & \_f, const Args &... \_xi) <br> |
|  T | [**eval**](classcip_1_1LinearInterpND.md#function-eval) (const Args &... args) const<br> |
|  Vector | [**evaln**](classcip_1_1LinearInterpND.md#function-evaln) (const Vectors &... inputs) const<br> |
|   | [**~LinearInterpND**](classcip_1_1LinearInterpND.md#function-linearinterpnd) () <br> |






















































## Public Functions Documentation




### function LinearInterp4D 

```C++
inline explicit cip::LinearInterp4D::LinearInterp4D (
    const Vector & x,
    const Vector & y,
    const Vector & z,
    const Vector & w,
    const Vector4 & f
) 
```




<hr>



### function ~LinearInterp4D 

```C++
inline cip::LinearInterp4D::~LinearInterp4D () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/linear_interp.hpp`

