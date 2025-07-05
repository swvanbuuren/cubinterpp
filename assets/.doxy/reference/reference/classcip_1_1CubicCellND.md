

# Class cip::CubicCellND

**template &lt;typename T, std::size\_t N&gt;**



[**ClassList**](annotated.md) **>** [**cip**](namespacecip.md) **>** [**CubicCellND**](classcip_1_1CubicCellND.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CubicCellND**](#function-cubiccellnd) (const Mdspan & F, const Spans & x) <br> |
|  T | [**eval**](#function-eval) (Args &&... xi) const<br> |
|   | [**~CubicCellND**](#function-cubiccellnd) () = default<br> |




























## Public Functions Documentation




### function CubicCellND 

```C++
inline explicit cip::CubicCellND::CubicCellND (
    const Mdspan & F,
    const Spans & x
) 
```




<hr>



### function eval 

```C++
template<typename... Args>
inline T cip::CubicCellND::eval (
    Args &&... xi
) const
```




<hr>



### function ~CubicCellND 

```C++
cip::CubicCellND::~CubicCellND () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/cubic_spline.hpp`

