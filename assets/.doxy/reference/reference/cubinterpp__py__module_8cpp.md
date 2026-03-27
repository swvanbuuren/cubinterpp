

# File cubinterpp\_py\_module.cpp



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**cubinterpp\_py\_module.cpp**](cubinterpp__py__module_8cpp.md)

[Go to the source code of this file](cubinterpp__py__module_8cpp_source.md)



* `#include <vector>`
* `#include <pybind11/stl.h>`
* `#include <pybind11/pybind11.h>`
* `#include <cubinterpp.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::vector&lt; double &gt; | [**DoubleVector**](#typedef-doublevector)  <br> |
| typedef std::vector&lt; DoubleVector &gt; | [**DoubleVector2**](#typedef-doublevector2)  <br> |
| typedef std::vector&lt; DoubleVector2 &gt; | [**DoubleVector3**](#typedef-doublevector3)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PYBIND11\_MODULE**](#function-pybind11_module) (cubinterpp\_py, m) <br> |




























## Public Types Documentation




### typedef DoubleVector 

```C++
using DoubleVector =  std::vector<double>;
```




<hr>



### typedef DoubleVector2 

```C++
using DoubleVector2 =  std::vector<DoubleVector>;
```




<hr>



### typedef DoubleVector3 

```C++
using DoubleVector3 =  std::vector<DoubleVector2>;
```




<hr>
## Public Functions Documentation




### function PYBIND11\_MODULE 

```C++
PYBIND11_MODULE (
    cubinterpp_py,
    m
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/cubinterpp_py_module.cpp`

