

# Class cip::LinearInterpND

**template &lt;typename T, std::size\_t N, IndexMethod IM&gt;**



[**ClassList**](annotated.md) **>** [**cip**](namespacecip.md) **>** [**LinearInterpND**](classcip_1_1LinearInterpND.md)










Inherited by the following classes: [cip::LinearInterp](classcip_1_1LinearInterp.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**LinearInterpND**](#function-linearinterpnd-12) (const [**F**](classcip_1_1VectorN.md) & \_f, const Args &... \_xi) <br> |
|   | [**LinearInterpND**](#function-linearinterpnd-22) ([**LinearInterpND**](classcip_1_1LinearInterpND.md) &&) = delete<br> |
|  T | [**eval**](#function-eval) (const Args &... args) const<br> |
|  Vector | [**evaln**](#function-evaln) (const Vectors &... inputs) const<br> |
|  [**LinearInterpND**](classcip_1_1LinearInterpND.md) & | [**operator=**](#function-operator) ([**LinearInterpND**](classcip_1_1LinearInterpND.md) &&) = delete<br> |
|   | [**~LinearInterpND**](#function-linearinterpnd) () <br> |




























## Public Functions Documentation




### function LinearInterpND [1/2]

```C++
template<typename... Args>
inline cip::LinearInterpND::LinearInterpND (
    const F & _f,
    const Args &... _xi
) 
```




<hr>



### function LinearInterpND [2/2]

```C++
cip::LinearInterpND::LinearInterpND (
    LinearInterpND &&
) = delete
```




<hr>



### function eval 

```C++
template<typename... Args>
inline T cip::LinearInterpND::eval (
    const Args &... args
) const
```




<hr>



### function evaln 

```C++
template<typename... Vectors>
inline Vector cip::LinearInterpND::evaln (
    const Vectors &... inputs
) const
```




<hr>



### function operator= 

```C++
LinearInterpND & cip::LinearInterpND::operator= (
    LinearInterpND &&
) = delete
```




<hr>



### function ~LinearInterpND 

```C++
inline cip::LinearInterpND::~LinearInterpND () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/linear_interp.hpp`

