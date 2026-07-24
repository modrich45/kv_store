add_library(
    kv_store_lib
    kv/kv_store.cpp
)

target_include_directories(
    kv_store_lib
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)