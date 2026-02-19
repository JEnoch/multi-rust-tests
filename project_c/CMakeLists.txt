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

# Both libproject_a.a and libproject_b.a embed their own copy of the Rust standard
# library (compiled with rustc 1.75.0 and 1.88.0 respectively). When both crates
# share the same dependencies (libc + rand), two strong (STB_GLOBAL) symbols collide
# at static link time:
#
#   - rust_eh_personality
#       The Rust/C++ exception-handling personality routine, defined as a strong
#       symbol in both std copies. LLVM does not allow renaming it, so it cannot
#       be mangled away. Tracking issue for a long-term fix:
#       https://github.com/rust-lang/rust/issues/104707
#
#   - __rust_no_alloc_shim_is_unstable
#       A sentinel BSS variable introduced in Rust 1.71 to guard the allocator shim.
#       Also strong in both archives. Tracking issue:
#       https://github.com/rust-lang/rust/issues/123015
#
# rustc PR #127173 mangled most other internal symbols (per-compiler-version hash),
# which eliminated the bulk of duplicate-symbol errors. But the two above are
# explicitly excluded from that mangling and remain unmangled in every rustc version:
#   https://github.com/rust-lang/rust/pull/127173
#
# Workaround (GNU ld only, Linux/ELF):
#   --allow-multiple-definition  tells GNU ld to keep the first definition and
#                                silently discard subsequent ones. Safe here because
#                                both implementations have identical semantics.
#   --start-group / --end-group  resolve circular archive references (belt-and-
#                                suspenders; not strictly required for these two libs).
#
# Note: this flag is a static-linker concern only. For the .so case (demo_dynamic),
# ld.so hides all std symbols inside each cdylib via an ELF version-script
# (local: *), so they never appear in .dynsym and no interposition occurs.
# See: https://alanwu.space/post/symbol-hygiene/
target_link_libraries(demo_static PRIVATE
    -Wl,--allow-multiple-definition
    -Wl,--start-group
    project_a_static
    project_b_static
    -Wl,--end-group
    pthread
    dl
    m
)

set_target_properties(demo_static PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_dependencies(demo_static project_a_cargo project_b_cargo)
