use rand::Rng;

/// C-compatible struct returned by project_b_random_u32().
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct ProjectBRandom {
    pub value: u32,
}

/// Returns a random u32 via the rand crate.
#[no_mangle]
pub extern "C" fn project_b_random_u32() -> ProjectBRandom {
    ProjectBRandom {
        value: rand::thread_rng().gen::<u32>(),
    }
}
