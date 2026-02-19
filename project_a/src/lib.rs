use libc::pid_t;

/// C-compatible struct returned by project_a_get_pid().
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct ProjectAPid {
    pub value: pid_t,
}

/// Returns the PID of the current process via libc::getpid().
#[no_mangle]
pub extern "C" fn project_a_get_pid() -> ProjectAPid {
    // SAFETY: getpid() has no preconditions and cannot fail.
    ProjectAPid {
        value: unsafe { libc::getpid() },
    }
}
