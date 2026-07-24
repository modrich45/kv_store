include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(
    kv_store_tests
    kv_store_test.cpp
)

target_link_libraries(
    kv_store_tests
    PRIVATE
    kv_store_lib
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(kv_store_tests)