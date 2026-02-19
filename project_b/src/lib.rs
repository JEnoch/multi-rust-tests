use libc::pid_t;
use rand::Rng;

/// C-compatible struct returned by project_b_get_pid().
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct ProjectBPid {
    pub value: pid_t,
}

/// C-compatible struct returned by project_b_random_u32().
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct ProjectBRandom {
    pub value: u32,
}

/// Returns the PID of the current process via libc::getpid().
#[no_mangle]
pub extern "C" fn project_b_get_pid() -> ProjectBPid {
    // SAFETY: getpid() has no preconditions and cannot fail.
    ProjectBPid {
        value: unsafe { libc::getpid() },
    }
}

/// Returns a random u32 via the rand crate.
#[no_mangle]
pub extern "C" fn project_b_random_u32() -> ProjectBRandom {
    ProjectBRandom {
        value: rand::thread_rng().gen::<u32>(),
    }
}
