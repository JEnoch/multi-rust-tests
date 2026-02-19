#include <stdio.h>

#include "project_a.h"
#include "project_b.h"

int main(void) {
    ProjectAPid    a_pid = project_a_get_pid();
    ProjectARandom a_rnd = project_a_random_u32();
    ProjectBPid    b_pid = project_b_get_pid();
    ProjectBRandom b_rnd = project_b_random_u32();

    printf("=== multi-rust-tests demo ===\n");
    printf("project_a (Rust 1.75.0): PID = %d\n",        (int)a_pid.value);
    printf("project_a (Rust 1.75.0): random u32 = %u\n", a_rnd.value);
    printf("project_b (Rust 1.88.0): PID = %d\n",        (int)b_pid.value);
    printf("project_b (Rust 1.88.0): random u32 = %u\n", b_rnd.value);

    return 0;
}
