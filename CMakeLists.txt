cmake_minimum_required(VERSION 3.4...3.18)
project(cubic_spline LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ version selection")

include_directories(extern)
set(PYTHON_EXECUTABLE "/mnt/storage/work/python/00__venvs/py38/bin/python")
set(PYBIND11_PYTHON_VERSION 3.8)
add_subdirectory(extern/pybind11)

pybind11_add_module(cubic_spline "")
# sources will be added in the following subdirectories
add_subdirectory(src)

add_custom_command(TARGET cubic_spline POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:cubic_spline> ${PROJECT_SOURCE_DIR}/example/$<TARGET_FILE_NAME:cubic_spline>
)
