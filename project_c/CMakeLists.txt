cmake_minimum_required(VERSION 3.21)

# --- demo_dynamic: dynamic link with .so from A and B  ---
add_executable(demo_dynamic ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)

target_link_libraries(demo_dynamic PRIVATE
    project_a_shared
    project_b_shared
)

set_target_properties(demo_dynamic PROPERTIES
    BUILD_RPATH "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_dependencies(demo_dynamic project_a_cargo project_b_cargo)


# --- demo_static: static link with .a from A and B ---
add_executable(demo_static ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)

target_link_libraries(demo_static PRIVATE
    project_a_static
    project_b_static
    pthread
    dl
    m
)

set_target_properties(demo_static PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_dependencies(demo_static project_a_cargo project_b_cargo)
