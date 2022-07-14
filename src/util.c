#include "util.h"

#include <stdint.h>

#include "la.h"
#include "structs.h"

float lerp(float a, float b, float t) { return a * t + b * (1 - t); }

float sigf(float a) { return (a > 0) ? 1 : ((a < 0) ? -1 : 0); }

float absf(float a) { return a * sigf(a); }

float clampf(float a, float mn, float mx) {
    return a < mn ? mn : (a > mx ? mx : a);
}

#ifdef WASM
void memset(void* p, int v, size_t s) {
    for (size_t i = 0; i < s; i++) {
        ((char*)p)[i] = v;
    }
}
#else
#include <string.h>
#endif

char __scratch_buff[MAX_SCRATCH_LENGTH];
char* itoa(size_t a) {
    char* ptr = __scratch_buff + MAX_SCRATCH_LENGTH - 1;
    *ptr = '\0';

    if (a == 0) *(--ptr) = '0';

    while (a) {
        *(--ptr) = '0' + a % 10;
        a /= 10;
    }

    return ptr;
}

char* ftoa(float f) {
    char* ptr = __scratch_buff + MAX_SCRATCH_LENGTH - 1;
    *ptr = '\0';

    uint64_t a = f * 1e5;
    for (size_t i = 0; i < 5; i++) {
        *(--ptr) = '0' + a % 10;
        a /= 10;
    }

    *(--ptr) = '.';

    if (a == 0) *(--ptr) = '0';

    while (a) {
        *(--ptr) = '0' + a % 10;
        a /= 10;
    }

    return ptr;
}

AABB get_camera_aabb() {
    return (AABB){st.camera.center, v2scale(st.screen_size, 0.5f)};
}

void get_aabb_tile_corners(AABB* a, V2* mn, V2* mx) {
    *mn = v2scale(v2sub(a->center, a->half_size), 1.f / TILE_SIZE);
    *mx = v2scale(v2add(a->center, a->half_size), 1.f / TILE_SIZE);
    *mn = v2clamp(*mn, v2(0, 0), v2(WORLD_SIZE, WORLD_SIZE));
    *mx = v2clamp(*mx, v2(0, 0), v2(WORLD_SIZE, WORLD_SIZE));
}

float qsqrtf(float x) {
    float l = 0, r = x;
    for (size_t i = 0; i < 8; i++) {
        float m = (l + r) * 0.5f;
        r = m * m > x ? m : r;
        l = m * m > x ? l : m;
    }
    l = (l + r) * 0.5f;
    l -= (l * l - x) / (2 * l);
    l -= (l * l - x) / (2 * l);
    l -= (l * l - x) / (2 * l);

    return l;
}
