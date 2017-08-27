#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include <assert.h>
#include <string.h>

void substring(char *src, int from, int n, char *dst);

#ifdef __cplusplus
}
#endif

#endif /* STRING_UTILS_H */

