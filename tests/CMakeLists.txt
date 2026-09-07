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

function(cubinterpp_add_test_single_header name)
    if(NOT DEFINED ARGUMENT_SOURCE)
        set(ARGUMENT_SOURCE ${name}.cpp)
    endif()
    set(single_header_target "${name}_single_header")
    add_executable(${single_header_target} ${ARGUMENT_SOURCE})
    target_link_libraries(${single_header_target} cubinterpp_single_header_lib gtest_main)
    # Ensure single header is generated before compiling this test
    add_dependencies(${single_header_target} cubinterpp_single_header)
    add_test(NAME ${single_header_target} COMMAND ${single_header_target})
    add_custom_command(TARGET ${single_header_target} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:${single_header_target}> ${PROJECT_SOURCE_DIR}/bin/$<TARGET_FILE_NAME:${single_header_target}>
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

# Define the list of tests to avoid duplication
set(CUBINTERPP_TESTS
    test_utils
    test_vectorn
    test_linear_interp
    test_cubic_splines_1d
    test_cubic_splines_2d
    test_cubic_splines_3d
    test_derivatives
    test_cubic_cell_nd
    test_cubic_interp_nd
)

# Add regular tests
foreach(test ${CUBINTERPP_TESTS})
    cubinterpp_add_test(${test})
endforeach()

# Add single header tests if enabled
if(CUBINTERPP_TEST_SINGLE_HEADER)
    foreach(test ${CUBINTERPP_TESTS})
        cubinterpp_add_test_single_header(${test})
    endforeach()
endif()
