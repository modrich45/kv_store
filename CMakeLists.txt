cmake_minimum_required(VERSION 3.20)
project(KVSTORE VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
    kv-store
    src/main.cpp
    src/kv/kv_store.cpp
)

target_include_directories(
    kv-store
    PRIVATE
    include
)