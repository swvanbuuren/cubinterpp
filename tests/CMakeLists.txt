function(cubinterpp_add_test name)
    if(NOT DEFINED ARGUMENT_SOURCE)
        set(ARGUMENT_SOURCE ${name}.cpp)
    endif()
    add_executable(${name} ${ARGUMENT_SOURCE})
    target_link_libraries(${name} cubinterpp gtest_main)
    add_test(NAME ${name} COMMAND ${name})
    add_custom_command(TARGET ${name} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:${name}> ${PROJECT_SOURCE_DIR}/bin/$<TARGET_FILE_NAME:${name}>
    )
endfunction()

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        b514bdc898e2951020cbdca1304b75f5950d1f59 # Version 1.15.2
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(googletest)

cubinterpp_add_test(test_vectorn)
cubinterpp_add_test(test_linear_interp)
cubinterpp_add_test(test_cubic_splines_1d)
cubinterpp_add_test(test_cubic_splines_2d)
cubinterpp_add_test(test_derivatives)
