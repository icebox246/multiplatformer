#ifndef __UTIL_H
#define __UTIL_H

#include "game.h"
#include "structs.h"
#include "globals.h"

float sigf(float a);
float absf(float);
float lerp(float a, float b, float t);
float clampf(float a, float mn, float mx);

void memset(void* p, int v, size_t s);

#define MAX_INT_LENGTH 20
extern char __int_conv_buff[MAX_INT_LENGTH];
char* itoa(size_t a);

AABB get_camera_aabb();
void get_aabb_tile_corners(AABB* a, V2* mn, V2* mx);


#endif  // __UTIL_H
