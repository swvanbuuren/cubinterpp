

# Class cip::VectorN

**template &lt;typename T, std::size\_t N&gt;**



[**ClassList**](annotated.md) **>** [**cip**](namespacecip.md) **>** [**VectorN**](classcip_1_1VectorN.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**VectorN**](#function-vectorn-14) (const T & initial\_value, const IndexArray & dimensions) <br> |
|   | [**VectorN**](#function-vectorn-24) (const [**VectorN**](classcip_1_1VectorN.md) & other) <br> |
|   | [**VectorN**](#function-vectorn-34) (const NestedVector & nested) <br> |
|   | [**VectorN**](#function-vectorn-44) (const IndexArray & dimensions) <br> |
|  const std::vector&lt; T &gt; & | [**data**](#function-data) () const<br> |
|  const IndexArray & | [**dimensions**](#function-dimensions) () const<br> |
|  void | [**emplace\_back**](#function-emplace_back) (Args... args) <br> |
|  Mdspan | [**get\_mdspan**](#function-get_mdspan-12) () <br> |
|  const Mdspan | [**get\_mdspan**](#function-get_mdspan-22) () const<br> |
|  void | [**move\_into\_submdspan**](#function-move_into_submdspan-12) (std::vector&lt; T &gt; && source, SliceSpecs &&... specs) <br> |
|  void | [**move\_into\_submdspan**](#function-move_into_submdspan-22) ([**VectorN**](classcip_1_1VectorN.md)&lt; T, M &gt; && source, SliceSpecs &&... specs) <br> |
|  T & | [**operator()**](#function-operator) (Indices... indices) <br> |
|  const T & | [**operator()**](#function-operator_1) (Indices... indices) const<br> |
|  T & | [**operator()**](#function-operator_2) (IndexArray & indices) <br> |
|  const T & | [**operator()**](#function-operator_3) (IndexArray & indices) const<br> |
|  Mdspan | [**submdspan**](#function-submdspan-12) (Pairs &&... pairs) const<br> |
|  Mdspan | [**submdspan**](#function-submdspan-22) (Pairs &&... pairs) <br> |
|  Mdspan1D | [**submdspan\_1d**](#function-submdspan_1d) (SliceArgs &&... args) <br> |




























## Public Functions Documentation




### function VectorN [1/4]

```C++
inline cip::VectorN::VectorN (
    const T & initial_value,
    const IndexArray & dimensions
) 
```




<hr>



### function VectorN [2/4]

```C++
inline cip::VectorN::VectorN (
    const VectorN & other
) 
```




<hr>



### function VectorN [3/4]

```C++
template<typename NestedVector>
inline cip::VectorN::VectorN (
    const NestedVector & nested
) 
```




<hr>



### function VectorN [4/4]

```C++
inline cip::VectorN::VectorN (
    const IndexArray & dimensions
) 
```




<hr>



### function data 

```C++
inline const std::vector< T > & cip::VectorN::data () const
```




<hr>



### function dimensions 

```C++
inline const IndexArray & cip::VectorN::dimensions () const
```




<hr>



### function emplace\_back 

```C++
template<typename... Args>
inline void cip::VectorN::emplace_back (
    Args... args
) 
```




<hr>



### function get\_mdspan [1/2]

```C++
inline Mdspan cip::VectorN::get_mdspan () 
```




<hr>



### function get\_mdspan [2/2]

```C++
inline const Mdspan cip::VectorN::get_mdspan () const
```




<hr>



### function move\_into\_submdspan [1/2]

```C++
template<typename... SliceSpecs>
inline void cip::VectorN::move_into_submdspan (
    std::vector< T > && source,
    SliceSpecs &&... specs
) 
```




<hr>



### function move\_into\_submdspan [2/2]

```C++
template<std::size_t M, typename... SliceSpecs>
inline void cip::VectorN::move_into_submdspan (
    VectorN < T, M > && source,
    SliceSpecs &&... specs
) 
```




<hr>



### function operator() 

```C++
template<typename... Indices>
inline T & cip::VectorN::operator() (
    Indices... indices
) 
```




<hr>



### function operator() 

```C++
template<typename... Indices>
inline const T & cip::VectorN::operator() (
    Indices... indices
) const
```




<hr>



### function operator() 

```C++
inline T & cip::VectorN::operator() (
    IndexArray & indices
) 
```




<hr>



### function operator() 

```C++
template<typename... Indices>
inline const T & cip::VectorN::operator() (
    IndexArray & indices
) const
```




<hr>



### function submdspan [1/2]

```C++
template<typename... Pairs>
inline Mdspan cip::VectorN::submdspan (
    Pairs &&... pairs
) const
```




<hr>



### function submdspan [2/2]

```C++
template<typename... Pairs>
inline Mdspan cip::VectorN::submdspan (
    Pairs &&... pairs
) 
```




<hr>



### function submdspan\_1d 

```C++
template<typename... SliceArgs>
inline Mdspan1D cip::VectorN::submdspan_1d (
    SliceArgs &&... args
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/vectorn.hpp`

