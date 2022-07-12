#include "util.h"

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

AABB get_camera_aabb() {
    return (AABB){st.camera.center, v2(WIDTH * 0.5f, HEIGHT * 0.5f)};
}

void get_aabb_tile_corners(AABB* a, V2* mn, V2* mx) {
    *mn = v2scale(v2sub(a->center, a->half_size), 1.f / TILE_SIZE);
    *mx = v2scale(v2add(a->center, a->half_size), 1.f / TILE_SIZE);
    *mn = v2clamp(*mn, v2(0, 0), v2(WORLD_SIZE, WORLD_SIZE));
    *mx = v2clamp(*mx, v2(0, 0), v2(WORLD_SIZE, WORLD_SIZE));
}
