use libc::pid_t;
use rand::Rng;
use rustc_version_runtime::version;

/// C-compatible struct returned by project_a_get_pid().
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct ProjectAPid {
    pub value: pid_t,
}

/// C-compatible struct returned by project_a_random_u32().
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct ProjectARandom {
    pub value: u32,
}

/// Returns the PID of the current process via libc::getpid().
#[no_mangle]
pub extern "C" fn project_a_get_pid() -> ProjectAPid {
    println!("     project_a_get_pid() was compiled using {:?}", version());
    // SAFETY: getpid() has no preconditions and cannot fail.
    ProjectAPid {
        value: unsafe { libc::getpid() },
    }
}

/// Returns a random u32 via the rand crate.
#[no_mangle]
pub extern "C" fn project_a_random_u32() -> ProjectARandom {
    println!("     project_a_random_u32() was compiled using {:?}", version());
    ProjectARandom {
        value: rand::thread_rng().gen::<u32>(),
    }
}
