fn main() {
    println!("cargo:rustc-cdylib-link-arg=-Wl,-soname,libproject_b.so");
}
