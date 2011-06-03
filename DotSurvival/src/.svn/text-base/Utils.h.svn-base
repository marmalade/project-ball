#pragma once

#ifndef DZ_UTILS_H
#define DZ_UTILS_H

#include "IwGeomFVec2.h"
#include "IwGeomSVec2.h"

#define wzx_ASSERT(x) IwAssert(0, x)
#define wzx_MemCopy(a, b, c) memcpy(b, a, c)
#define wzx_ReAlloc realloc
#define wzx_Malloc malloc
#define wzx_Free free


typedef  CIwFVec2 Vector2;
typedef  CIwSVec2 Vector2i;


extern const float pi;
extern const float half_pi;
extern const float two_pi;


Vector2 StringToVector2(const char* str, const char* strDelimiter);

inline float fClamp(float x, float a, float b)
{
    return x < a ? a : (x > b ? b : x);
}

inline int iClamp(int x, int a, int b)
{
    return x < a ? a : (x > b ? b : x);
}

int
intersect_triangle(float orig[3], float dir[3],
                   float vert0[3], float vert1[3], float vert2[3],
                   float *t, float *u, float *v);


void PrintfProperty(int16 x, int16 y, const char* str, float f);


#endif
