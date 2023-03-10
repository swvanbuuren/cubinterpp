cmake_minimum_required(VERSION 3.11...3.18)
project(cubic_spline LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ version selection")

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
FetchContent_MakeAvailable(pybind11 Eigen)


set(PYTHON_EXECUTABLE "/mnt/storage/work/python/00__venvs/py38/bin/python")
set(PYBIND11_PYTHON_VERSION 3.8)

pybind11_add_module(${CMAKE_PROJECT_NAME} "")
# sources will be added in the following subdirectories
add_subdirectory(src)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Eigen3::Eigen)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:cubic_spline> ${PROJECT_SOURCE_DIR}/example/$<TARGET_FILE_NAME:cubic_spline>
)
