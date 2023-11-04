# Circular Buffer - uint8_t interface.
define_test_suite(test_cb_uint8_t)
target_compile_definitions(test_cb_uint8_t PRIVATE "USE_UINT8_T")
target_sources(test_cb_uint8_t PRIVATE ${SOURCES_CB_ALL})
target_include_directories(test_cb_uint8_t PRIVATE ${INCLUDE_DIRS_CB_ALL})
target_sources(test_cb_uint8_t PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/test_cb.c")
target_include_directories(test_cb_uint8_t PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# Circular Buffer - uint16_t interface.
define_test_suite(test_cb_uint16_t)
target_compile_definitions(test_cb_uint16_t PRIVATE "USE_UINT16_T")
target_sources(test_cb_uint16_t PRIVATE ${SOURCES_CB_ALL})
target_include_directories(test_cb_uint16_t PRIVATE ${INCLUDE_DIRS_CB_ALL})
target_sources(test_cb_uint16_t PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/test_cb.c")
target_include_directories(test_cb_uint16_t PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# Circular Buffer - uint32_t interface.
define_test_suite(test_cb_uint32_t)
target_compile_definitions(test_cb_uint32_t PRIVATE "USE_UINT32_T")
target_sources(test_cb_uint32_t PRIVATE ${SOURCES_CB_ALL})
target_include_directories(test_cb_uint32_t PRIVATE ${INCLUDE_DIRS_CB_ALL})
target_sources(test_cb_uint32_t PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/test_cb.c")
target_include_directories(test_cb_uint32_t PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# Circular Buffer - uint64_t interface.
define_test_suite(test_cb_uint64_t)
target_compile_definitions(test_cb_uint64_t PRIVATE "USE_UINT64_T")
target_sources(test_cb_uint64_t PRIVATE ${SOURCES_CB_ALL})
target_include_directories(test_cb_uint64_t PRIVATE ${INCLUDE_DIRS_CB_ALL})
target_sources(test_cb_uint64_t PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/test_cb.c")
target_include_directories(test_cb_uint64_t PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# Circular Buffer - concurrency scenarios with threads, note threads are not available on every platform.
find_package(Threads)
if(${CMAKE_USE_PTHREADS_INIT})
    message(STATUS "'pthreads' compatible threads library found, concurrency tests added...")

    define_test_suite(test_cb_threads_uint8_t)
    target_compile_definitions(test_cb_threads_uint8_t PRIVATE "USE_UINT8_T")
    target_sources(test_cb_threads_uint8_t PRIVATE ${SOURCES_CB_ALL})
    target_include_directories(test_cb_threads_uint8_t PRIVATE ${INCLUDE_DIRS_CB_ALL})
    target_sources(test_cb_threads_uint8_t PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/test_cb_threads.c")
    target_include_directories(test_cb_threads_uint8_t PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

    define_test_suite(test_cb_threads_uint16_t)
    target_compile_definitions(test_cb_threads_uint16_t PRIVATE "USE_UINT16_T")
    target_sources(test_cb_threads_uint16_t PRIVATE ${SOURCES_CB_ALL})
    target_include_directories(test_cb_threads_uint16_t PRIVATE ${INCLUDE_DIRS_CB_ALL})
    target_sources(test_cb_threads_uint16_t PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/test_cb_threads.c")
    target_include_directories(test_cb_threads_uint16_t PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

    define_test_suite(test_cb_threads_uint32_t)
    target_compile_definitions(test_cb_threads_uint32_t PRIVATE "USE_UINT32_T")
    target_sources(test_cb_threads_uint32_t PRIVATE ${SOURCES_CB_ALL})
    target_include_directories(test_cb_threads_uint32_t PRIVATE ${INCLUDE_DIRS_CB_ALL})
    target_sources(test_cb_threads_uint32_t PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/test_cb_threads.c")
    target_include_directories(test_cb_threads_uint32_t PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

    define_test_suite(test_cb_threads_uint64_t)
    target_compile_definitions(test_cb_threads_uint64_t PRIVATE "USE_UINT64_T")
    target_sources(test_cb_threads_uint64_t PRIVATE ${SOURCES_CB_ALL})
    target_include_directories(test_cb_threads_uint64_t PRIVATE ${INCLUDE_DIRS_CB_ALL})
    target_sources(test_cb_threads_uint64_t PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/test_cb_threads.c")
    target_include_directories(test_cb_threads_uint64_t PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
else()
    message(STATUS "No 'pthreads' compatible threads library found, concurrency tests skipped...")
endif()