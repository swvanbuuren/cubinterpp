cmake_minimum_required(VERSION 3.11...3.18)

project(cubinterpp 
    VERSION 0.1.0 
    LANGUAGES CXX
)

option(CUBINTERPP_ENABLE_TESTS "Enable tests." TRUE)

set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ version selection")

include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        0694ec6a15863bff2e0ea5efe07c78de39b9a33c # Version 2.10.2
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)

FetchContent_Declare(
  Eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG        3147391d946bb4b6c68edd901f2add6ac1f31f8c # Version 3.4.0
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)
set(EIGEN_BUILD_DOC OFF)
# note: To disable eigen tests,
# you should put this code in a add_subdirectory to avoid to change
# BUILD_TESTING for your own project too since variables are directory
# scoped
set(BUILD_TESTING OFF)
set(EIGEN_BUILD_PKGCONFIG OFF)

FetchContent_Declare(
  mdspan
  GIT_REPOSITORY https://github.com/kokkos/mdspan.git
  GIT_TAG stable
  #GIT_TAG 00448b8b594c40e5c5e576ade28820b2b26fd27c
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)
add_definitions(-DMDSPAN_IMPL_STANDARD_NAMESPACE=std)
set(MDSPAN_CXX_STANDARD ${CMAKE_CXX_STANDARD})

FetchContent_MakeAvailable(pybind11 Eigen mdspan)

pybind11_add_module(${CMAKE_PROJECT_NAME} "")
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Eigen3::Eigen std::mdspan)

add_library(cubinterpp_core INTERFACE)
target_link_libraries(cubinterpp_core INTERFACE std::mdspan)
target_include_directories(cubinterpp_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# sources will be added in the following subdirectories
add_subdirectory(src)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:cubinterpp> ${PROJECT_SOURCE_DIR}/cubinterpp/$<TARGET_FILE_NAME:cubinterpp>
)

if(CUBINTERPP_ENABLE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
