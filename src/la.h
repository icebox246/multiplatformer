#ifndef __LA_H
#define __LA_H
#include "structs.h"
#include "util.h"

V2 v2(float x, float y);
V2 v2zero();
V2 v2add(V2 a, V2 b);
V2 v2sub(V2 a, V2 b);
V2 v2mult(V2 a, V2 b);
V2 v2scale(V2 a, float c);
float v2len(V2 a);
V2 v2perp(V2 a);
V2 v2norm(V2 a);
V2 v2lerp(V2 a, V2 b, float t);

V2 v2clamp(V2 a, V2 mn, V2 mx);

float* v2axis(V2* v, int ax);
#define V2AXIS(v,ax) (*v2axis(&(v),ax))

#endif
