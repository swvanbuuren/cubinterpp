

# Class cip::LinearInterpND

**template &lt;typename T, std::size\_t N&gt;**



[**ClassList**](annotated.md) **>** [**cip**](namespacecip.md) **>** [**LinearInterpND**](classcip_1_1LinearInterpND.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**LinearInterpND**](#function-linearinterpnd) (const [**F**](classcip_1_1VectorN.md) & \_f, const Args &... \_xi) <br> |
|  T | [**eval**](#function-eval) (const Args &... args) const<br> |
|  Vector | [**evaln**](#function-evaln) (const Vectors &... inputs) const<br> |
|   | [**~LinearInterpND**](#function-linearinterpnd) () <br> |




























## Public Functions Documentation




### function LinearInterpND 

```C++
template<typename... Args>
inline cip::LinearInterpND::LinearInterpND (
    const F & _f,
    const Args &... _xi
) 
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



### function ~LinearInterpND 

```C++
inline cip::LinearInterpND::~LinearInterpND () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/linear_interp.hpp`

