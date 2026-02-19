#ifndef PROJECT_A_H
#define PROJECT_A_H

#include <sys/types.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    pid_t value;
} ProjectAPid;

typedef struct {
    uint32_t value;
} ProjectARandom;

ProjectAPid   project_a_get_pid(void);
ProjectARandom project_a_random_u32(void);

#ifdef __cplusplus
}
#endif

#endif /* PROJECT_A_H */
