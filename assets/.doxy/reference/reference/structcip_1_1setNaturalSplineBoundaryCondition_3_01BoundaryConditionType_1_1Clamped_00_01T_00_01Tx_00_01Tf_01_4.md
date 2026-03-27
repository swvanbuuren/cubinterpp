

# Struct cip::setNaturalSplineBoundaryCondition&lt; BoundaryConditionType::Clamped, T, Tx, Tf &gt;

**template &lt;typename T, typename Tx, typename Tf&gt;**



[**ClassList**](annotated.md) **>** [**cip**](namespacecip.md) **>** [**setNaturalSplineBoundaryCondition&lt; BoundaryConditionType::Clamped, T, Tx, Tf &gt;**](structcip_1_1setNaturalSplineBoundaryCondition_3_01BoundaryConditionType_1_1Clamped_00_01T_00_01Tx_00_01Tf_01_4.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::vector&lt; T &gt; | [**Vector**](#typedef-vector)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  constexpr void | [**operator()**](#function-operator) (const Tx & x, const Tf & f, Vector & a, Vector & b, Vector & c, Vector & d) const<br> |




























## Public Types Documentation




### typedef Vector 

```C++
using cip::setNaturalSplineBoundaryCondition< BoundaryConditionType::Clamped, T, Tx, Tf >::Vector =  std::vector<T>;
```




<hr>
## Public Functions Documentation




### function operator() 

```C++
inline constexpr void cip::setNaturalSplineBoundaryCondition< BoundaryConditionType::Clamped, T, Tx, Tf >::operator() (
    const Tx & x,
    const Tf & f,
    Vector & a,
    Vector & b,
    Vector & c,
    Vector & d
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/slopes.hpp`

