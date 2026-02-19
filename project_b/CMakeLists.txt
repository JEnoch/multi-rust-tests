cmake_minimum_required(VERSION 3.21)

set(B_SRC ${CMAKE_CURRENT_SOURCE_DIR})
set(B_TARGET_DIR ${CMAKE_BINARY_DIR}/cargo_target_b)
set(B_RELEASE ${B_TARGET_DIR}/release)
set(B_STATIC ${B_RELEASE}/libproject_b.a)
set(B_SHARED ${B_RELEASE}/libproject_b.so)
set(B_STATIC_DEST ${CMAKE_BINARY_DIR}/lib/libproject_b.a)
set(B_SHARED_DEST ${CMAKE_BINARY_DIR}/lib/libproject_b.so)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_custom_command(
    OUTPUT ${B_STATIC} ${B_SHARED}
    COMMAND
        ${CMAKE_COMMAND} -E env
            RUSTUP_TOOLCHAIN=1.88.0
            CARGO_TARGET_DIR=${B_TARGET_DIR}
        cargo build --release --manifest-path ${B_SRC}/Cargo.toml
    COMMAND ${CMAKE_COMMAND} -E copy ${B_STATIC} ${B_STATIC_DEST}
    COMMAND ${CMAKE_COMMAND} -E copy ${B_SHARED} ${B_SHARED_DEST}
    DEPENDS
        ${B_SRC}/Cargo.toml
        ${B_SRC}/build.rs
        ${B_SRC}/src/lib.rs
    COMMENT "Building project_b with Rust 1.88.0"
    VERBATIM
)

add_custom_target(project_b_cargo ALL
    DEPENDS ${B_STATIC} ${B_SHARED}
)

add_library(project_b_static STATIC IMPORTED GLOBAL)
set_target_properties(project_b_static PROPERTIES
    IMPORTED_LOCATION ${B_STATIC_DEST}
    INTERFACE_INCLUDE_DIRECTORIES ${B_SRC}/include
)
add_dependencies(project_b_static project_b_cargo)

add_library(project_b_shared SHARED IMPORTED GLOBAL)
set_target_properties(project_b_shared PROPERTIES
    IMPORTED_LOCATION ${B_SHARED_DEST}
    IMPORTED_SONAME libproject_b.so
    INTERFACE_INCLUDE_DIRECTORIES ${B_SRC}/include
)
add_dependencies(project_b_shared project_b_cargo)
