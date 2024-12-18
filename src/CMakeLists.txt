target_sources(cubinterpp PRIVATE
    cubinterpp.cpp
    cubic_spline.hpp
    linear_interp.hpp
    vectorn.hpp
 )

target_sources(cubinterpp_core INTERFACE
    cubic_spline.hpp
    linear_interp.hpp
    vectorn.hpp
)
