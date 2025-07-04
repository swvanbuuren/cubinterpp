cmake_minimum_required(VERSION 3.15...3.29)

project(cubinterpp 
    VERSION 0.1.0 
    LANGUAGES CXX
)

option(CUBINTERPP_ENABLE_TESTS "Enable tests." TRUE)
option(CUBINTERPP_ENABLE_COMPILE_OPTIMIZATIONS "Enable all optimizations." FALSE)
option(CUBINTERPP_ENABLE_CONCEPTS "Enable concepts." FALSE)

set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ version selection")

include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        a2e59f0e7065404b44dfe92a28aca47ba1378dc4 # Version v2.13.6
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
set(MDSPAN_GENERATE_STD_NAMESPACE_TARGETS On)

FetchContent_MakeAvailable(pybind11 mdspan)

add_library(${CMAKE_PROJECT_NAME} INTERFACE)
target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE std::mdspan)
target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

pybind11_add_module(cubinterpp_py "")
target_link_libraries(cubinterpp_py PRIVATE ${CMAKE_PROJECT_NAME})
target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

if (CUBINTERPP_ENABLE_COMPILE_OPTIMIZATIONS)
  add_compile_options(-O3)
endif()
if (CUBINTERPP_ENABLE_CONCEPTS)
  add_compile_options(-fconcepts)
endif()

# sources will be added in the following subdirectories
add_subdirectory(src)

add_custom_command(TARGET cubinterpp_py POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:cubinterpp_py> ${PROJECT_SOURCE_DIR}/cubinterpp/$<TARGET_FILE_NAME:cubinterpp_py>
)

if(CUBINTERPP_ENABLE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
