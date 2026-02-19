#ifndef PROJECT_B_H
#define PROJECT_B_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t value;
} ProjectBRandom;

ProjectBRandom project_b_random_u32(void);

#ifdef __cplusplus
}
#endif

#endif /* PROJECT_B_H */
