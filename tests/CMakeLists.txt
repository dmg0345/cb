# @brief Creates a test suite for 'cmocka' and adds it to 'ctest', each test suite maps to an executable target.
# @param[in] The name of the test suite to create.
function(define_test_suite TEST_SUITE_NAME)
    # Create executable for the test suite and link the test harness to it.
    add_executable(${TEST_SUITE_NAME})
    target_link_libraries(${TEST_SUITE_NAME} PRIVATE cmocka)
    install(TARGETS ${TEST_SUITE_NAME} RUNTIME DESTINATION "${CMAKE_INSTALL_RUNTIMEDIR}")

    # Add additional test source and header files to the test.
    target_sources(${TEST_SUITE_NAME} PRIVATE
        "${PROJECT_ROOT_DIR}/tests/tests/.test_utils/cmocka_defs.c"
        "${PROJECT_ROOT_DIR}/tests/tests/.test_utils/cb_evt_handlers/cb_evt_handlers.c"
    )
    target_include_directories(${TEST_SUITE_NAME} PRIVATE
        "${PROJECT_ROOT_DIR}/tests/tests/.test_utils"
    )

    # If running on CI, then enable XML output on CMocka, otherwise use the default standard output.
    if((DEFINED CFG_CI) AND (${CFG_CI}))
        target_compile_definitions(${TEST_SUITE_NAME} PRIVATE "CMOCKA_JUNIT_XML_OUTPUT")
    endif()

    # Add converage if enabled.
    if(${CFG_TESTS_ENABLE_COVERAGE} STREQUAL "ON")
        target_link_libraries(${TEST_SUITE_NAME} PRIVATE "gcov")
        target_compile_options(${TEST_SUITE_NAME} PRIVATE "-ftest-coverage" "-fprofile-arcs")
    endif()

    # Add test suite to CTest.
    add_test(NAME ${TEST_SUITE_NAME} COMMAND ${TEST_SUITE_NAME})
endfunction()

## CMocka Test Harness #################################################################################################
add_subdirectory("cmocka")

## Tests ###############################################################################################################
add_subdirectory("tests")
