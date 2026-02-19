#include <stdio.h>

#include "project_a.h"
#include "project_b.h"

int main(void) {
    ProjectAPid  pid = project_a_get_pid();
    ProjectBRandom rnd = project_b_random_u32();

    printf("=== multi-rust-tests demo ===\n");
    printf("project_a (Rust 1.75.0, libc): PID = %d\n", (int)pid.value);
    printf("project_b (Rust 1.88.0, rand): random u32 = %u\n", rnd.value);

    return 0;
}
