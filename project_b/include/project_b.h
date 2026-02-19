#ifndef PROJECT_B_H
#define PROJECT_B_H

#include <sys/types.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    pid_t value;
} ProjectBPid;

typedef struct {
    uint32_t value;
} ProjectBRandom;

ProjectBPid    project_b_get_pid(void);
ProjectBRandom project_b_random_u32(void);

#ifdef __cplusplus
}
#endif

#endif /* PROJECT_B_H */
