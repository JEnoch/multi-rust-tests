# multi-rust-tests

Proof of concept demonstrating that two Rust libraries compiled with **different Rust toolchain versions** can coexist in the same C project, linked both dynamically and statically.

## Project Structure

```
multi-rust-tests/
├── Dockerfile          # Ubuntu noble + rustup (apt) + toolchains 1.75.0 & 1.88.0
├── compose.yml         # Docker Compose service with workspace volume
├── CMakeLists.txt      # Root CMake: orchestrates A, B, C
├── project_a/          # Rust 1.75.0 — libc crate — exposes project_a_get_pid()
├── project_b/          # Rust 1.88.0 — rand crate  — exposes project_b_random_u32()
└── project_c/          # C main — calls both functions, built as demo_dynamic & demo_static
```

### project_a

- Rust toolchain: **1.75.0**
- Dependency: [`libc`](https://crates.io/crates/libc)
- Exposed function: `ProjectAPid project_a_get_pid(void)` — wraps `libc::getpid()`
- Output: `libproject_a.so` + `libproject_a.a`

### project_b

- Rust toolchain: **1.88.0**
- Dependency: [`rand`](https://crates.io/crates/rand) 0.8
- Exposed function: `ProjectBRandom project_b_random_u32(void)` — generates a random `u32`
- Output: `libproject_b.so` + `libproject_b.a`

### project_c

Builds two executables:

| Executable     | Linking                              |
|----------------|--------------------------------------|
| `demo_dynamic` | Dynamic (`.so`) — absolute RPATH     |
| `demo_static`  | Static (`.a`) — `pthread`, `dl`, `m` |

## Prerequisites

- [Docker](https://docs.docker.com/get-docker/)
- [Docker Compose](https://docs.docker.com/compose/install/) (v2, bundled with Docker Desktop)

## Usage

### 1. Build the Docker image

```bash
docker compose build
```

This installs `build-essential`, `cmake`, `rustup` (via apt), and both Rust toolchains (1.75.0 and 1.88.0) inside the image.

### 2. Start an interactive container

```bash
docker compose run --rm builder bash
```

The project root is mounted at `/workspace` inside the container.

### 3. Build the project (inside the container)

```bash
cmake -S /workspace -B /workspace/build -DCMAKE_BUILD_TYPE=Release
cmake --build /workspace/build --parallel $(nproc)
```

CMake calls `cargo build --release` separately for each Rust project, passing `RUSTUP_TOOLCHAIN` via environment variable to enforce the correct toolchain version.

### 4. Inspect the artifacts

```bash
ls /workspace/build/lib/
# libproject_a.a  libproject_a.so  libproject_b.a  libproject_b.so

ls /workspace/build/bin/
# demo_dynamic  demo_static
```

### 5. Run the demos

```bash
/workspace/build/bin/demo_dynamic
/workspace/build/bin/demo_static
```

Expected output (values will vary):

```
=== multi-rust-tests demo ===
project_a (Rust 1.75.0, libc): PID = 42
project_b (Rust 1.88.0, rand): random u32 = 3748291847
```

### 6. Verify toolchain versions in the built libraries

```bash
strings /workspace/build/lib/libproject_a.a | grep "rustc 1.75"
strings /workspace/build/lib/libproject_b.a | grep "rustc 1.88"
```

### 7. Verify dynamic linking (RPATH and SONAME)

```bash
# Check embedded RPATH
readelf -d /workspace/build/bin/demo_dynamic | grep RPATH

# Check DT_NEEDED entries (should show libproject_a.so and libproject_b.so, not paths)
readelf -d /workspace/build/bin/demo_dynamic | grep NEEDED

# Check shared library resolution at runtime
ldd /workspace/build/bin/demo_dynamic | grep project
```

---

## Implementation notes

### rustup via apt and `--no-self-update`

The Ubuntu noble `rustup` apt package installs the rustup binary at `/usr/bin/rustup`. After installing a toolchain, rustup attempts to update its own binary in `~/.cargo/bin/rustup` — which fails because the apt-managed binary cannot self-update.

The fix is to pass `--no-self-update` to each `rustup toolchain install` call. A symlink `~/.cargo/bin/rustup -> /usr/bin/rustup` is also created beforehand so that the proxy binaries (`cargo`, `rustc`, etc.) installed by rustup can locate the rustup binary.

### SONAME and dynamic linking

Rust `cdylib` crates do not embed a SONAME in the produced `.so` by default. Without a SONAME, the linker records the path passed to it (e.g., `../lib/libproject_a.so`) as the `DT_NEEDED` entry in the executable. Because `DT_NEEDED` entries containing a slash are treated as direct paths by the dynamic linker — bypassing RPATH entirely — the library cannot be found at runtime.

Each Rust project includes a `build.rs` that passes `-Wl,-soname,libproject_X.so` to the linker:

```rust
fn main() {
    println!("cargo:rustc-cdylib-link-arg=-Wl,-soname,libproject_a.so");
}
```

With a SONAME embedded, `DT_NEEDED` contains just `libproject_a.so` (no slash), and the RPATH is used normally to resolve it.

### Static linking and duplicate symbols

Both `.a` archives embed their own copy of the Rust standard library. This does not cause duplicate symbol errors because Rust deliberately marks most internal std symbols as **weak** (`STB_WEAK`) in `staticlib` outputs. The linker keeps the first definition and silently discards subsequent ones — no special flags required.

The external system libraries `pthread`, `dl`, and `m` are still needed because the Rust std runtime uses threads, dynamic loading, and math functions.

> `lto = false` is set in both `Cargo.toml` profiles as a precaution: with LTO enabled, rustc embeds LLVM bitcode in object files, which can interfere with weak symbol deduplication at link time.
