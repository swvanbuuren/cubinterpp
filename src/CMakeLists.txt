target_sources(cubinterpp_py PRIVATE
    cubinterpp.cpp
    cubic_spline.hpp
    linear_interp.hpp
    vectorn.hpp
    utils.hpp
 )

target_sources(${CMAKE_PROJECT_NAME} INTERFACE
    cubic_spline.hpp
    linear_interp.hpp
    vectorn.hpp
    utils.hpp
)
