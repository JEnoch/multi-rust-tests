cmake_minimum_required(VERSION 3.21)

set(A_SRC ${CMAKE_CURRENT_SOURCE_DIR})
set(A_TARGET_DIR ${CMAKE_BINARY_DIR}/cargo_target_a)
set(A_RELEASE ${A_TARGET_DIR}/release)
set(A_STATIC ${A_RELEASE}/libproject_a.a)
set(A_SHARED ${A_RELEASE}/libproject_a.so)
set(A_STATIC_DEST ${CMAKE_BINARY_DIR}/lib/libproject_a.a)
set(A_SHARED_DEST ${CMAKE_BINARY_DIR}/lib/libproject_a.so)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_custom_command(
    OUTPUT ${A_STATIC} ${A_SHARED}
    COMMAND
        ${CMAKE_COMMAND} -E env
            RUSTUP_TOOLCHAIN=1.75.0
            CARGO_TARGET_DIR=${A_TARGET_DIR}
        cargo build --release --manifest-path ${A_SRC}/Cargo.toml
    COMMAND ${CMAKE_COMMAND} -E copy ${A_STATIC} ${A_STATIC_DEST}
    COMMAND ${CMAKE_COMMAND} -E copy ${A_SHARED} ${A_SHARED_DEST}
    DEPENDS
        ${A_SRC}/Cargo.toml
        ${A_SRC}/build.rs
        ${A_SRC}/src/lib.rs
    COMMENT "Building project_a with Rust 1.75.0"
    VERBATIM
)

add_custom_target(project_a_cargo ALL
    DEPENDS ${A_STATIC} ${A_SHARED}
)

add_library(project_a_static STATIC IMPORTED GLOBAL)
set_target_properties(project_a_static PROPERTIES
    IMPORTED_LOCATION ${A_STATIC_DEST}
    INTERFACE_INCLUDE_DIRECTORIES ${A_SRC}/include
)
add_dependencies(project_a_static project_a_cargo)

add_library(project_a_shared SHARED IMPORTED GLOBAL)
set_target_properties(project_a_shared PROPERTIES
    IMPORTED_LOCATION ${A_SHARED_DEST}
    IMPORTED_SONAME libproject_a.so
    INTERFACE_INCLUDE_DIRECTORIES ${A_SRC}/include
)
add_dependencies(project_a_shared project_a_cargo)
