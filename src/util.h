#ifndef __UTIL_H
#define __UTIL_H

#include "game.h"
#include "structs.h"
#include "globals.h"

float sigf(float a);
float absf(float);
float lerp(float a, float b, float t);
float clampf(float a, float mn, float mx);

#ifdef WASM
void memset(void* p, int v, size_t s);
#endif

#define MAX_SCRATCH_LENGTH 64
extern char __scratch_buff[];
char* itoa(size_t a);
char* ftoa(float f);

AABB get_camera_aabb();
void get_aabb_tile_corners(AABB* a, V2* mn, V2* mx);

float qsqrtf(float x);


#endif  // __UTIL_H
