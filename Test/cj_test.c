#include <string.h>
#include "cj.h"

// TESTINGS A BROKEN CASE
int main(void) {
    char src[1<<14] = {0};
    memset(src, '[', sizeof(src)-1);

    CJ_Arena *a = cj_arena_create(0);
    cj_parse(a, src);
    cj_arena_free(a);

    return 0;
}