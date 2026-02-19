#ifndef PROJECT_A_H
#define PROJECT_A_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    pid_t value;
} ProjectAPid;

ProjectAPid project_a_get_pid(void);

#ifdef __cplusplus
}
#endif

#endif /* PROJECT_A_H */
