#include "la.h"

/* #include <math.h> */

V2 v2(float x, float y) {
    V2 res = {.x = x, .y = y};
    return res;
}

V2 v2zero() { return v2(0, 0); }

V2 v2add(V2 a, V2 b) {
    V2 res = {.x = a.x + b.x, .y = a.y + b.y};
    return res;
}

V2 v2sub(V2 a, V2 b) {
    V2 res = {.x = a.x - b.x, .y = a.y - b.y};
    return res;
}

V2 v2mult(V2 a, V2 b) {
    V2 res = {.x = a.x * b.x, .y = a.y * b.y};
    return res;
}

V2 v2scale(V2 a, float c) {
    V2 res = {.x = a.x * c, .y = a.y * c};
    return res;
}

V2 v2clamp(V2 a, V2 mn, V2 mx) {
    V2 res = {.x = clampf(a.x, mn.x, mx.x), .y = clampf(a.y, mn.y, mx.y)};
    return res;
}

// TODO: I'm probably gonna need this
/* float v2len(V2 a) { return (float)sqrt(a.x * a.x + a.y * a.y); } */

/* V2 v2norm(V2 a) { return v2scale(a, 1.0 / v2len(a)); } */

V2 v2perp(V2 a) {
    V2 res = {.x = a.y, .y = -a.x};
    return res;
}

V2 v2lerp(V2 a, V2 b, float t) {
    return v2(lerp(a.x, b.x, t), lerp(a.y, b.y, t));
}

