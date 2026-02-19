#include <stdio.h>

#include "project_a.h"
#include "project_b.h"

int main(void) {
    printf("=== multi-rust-tests demo ===\n");

    printf(" == Calling project_a ...\n");
    ProjectAPid    a_pid = project_a_get_pid();
    ProjectARandom a_rnd = project_a_random_u32();
    printf("   project_a: PID = %d\n",        (int)a_pid.value);
    printf("   project_a: random u32 = %u\n", a_rnd.value);

    printf(" == Calling project_b ...\n");
    ProjectBPid    b_pid = project_b_get_pid();
    ProjectBRandom b_rnd = project_b_random_u32();
    printf("   project_b: PID = %d\n",        (int)b_pid.value);
    printf("   project_b: random u32 = %u\n", b_rnd.value);

    return 0;
}
