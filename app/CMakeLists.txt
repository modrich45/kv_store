add_executable(
    kv_store
    main.cpp
)

target_link_libraries(
    kv_store
    PRIVATE
    kv_store_lib
)