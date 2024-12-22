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
  mdspan
  GIT_REPOSITORY https://github.com/kokkos/mdspan.git
  GIT_TAG stable
  #GIT_TAG 00448b8b594c40e5c5e576ade28820b2b26fd27c
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)
add_definitions(-DMDSPAN_IMPL_STANDARD_NAMESPACE=std)
set(MDSPAN_CXX_STANDARD ${CMAKE_CXX_STANDARD})

FetchContent_MakeAvailable(pybind11 mdspan)

pybind11_add_module(cubinterpp_py "")
target_link_libraries(cubinterpp_py PRIVATE std::mdspan)

add_library(${CMAKE_PROJECT_NAME} INTERFACE)
target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE std::mdspan)
target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE src)

# sources will be added in the following subdirectories
add_subdirectory(src)

add_custom_command(TARGET cubinterpp_py POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:cubinterpp_py> ${PROJECT_SOURCE_DIR}/cubinterpp/$<TARGET_FILE_NAME:cubinterpp_py>
)

if(CUBINTERPP_ENABLE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
