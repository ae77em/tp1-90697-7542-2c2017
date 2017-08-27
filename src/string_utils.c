#include "string_utils.h"

void substring(char *src, int from, int n, char *dst) {
    int length = strlen(src);
    int last_pos = from + n;

    //assert(from >= 0 && last_pos < length);
    if (!(from >= 0 && last_pos < length)) {
        last_pos++;
        last_pos--;
    }

    for (int i = 0; i < n; i++) {
        dst[i] = src[from + i];
    }

    dst[last_pos] = '\0';
}
