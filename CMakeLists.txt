cmake_minimum_required(VERSION 3.20)

project(KVSTORE VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(external/googletest)

# ---------------- Library ----------------

add_library(
    kvstore_lib
    src/kv/kv_store.cpp
)

target_include_directories(
    kvstore_lib
    PUBLIC
    include
)

# ---------------- Application ----------------

add_executable(
    kv-store
    src/main.cpp
)

target_link_libraries(
    kv-store
    PRIVATE
    kvstore_lib
)

# ---------------- Tests ----------------

add_executable(
    kv-store-tests
    tests/kv_store_test.cpp
)

target_link_libraries(
    kv-store-tests
    PRIVATE
    kvstore_lib
    GTest::gtest_main
)