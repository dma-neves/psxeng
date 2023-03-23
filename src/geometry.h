#ifndef GEOMETRY_H
#define GEOMETRY_H


#include "util.h"

#include <psxgte.h>
#include <psxgpu.h>

typedef struct Color
{
    int r;
    int g;
    int b;
} Color;

typedef struct Rect
{
    int x;
    int y;
    int width;
    int height;
    Color color;
} Rect;


typedef struct Object
{
    SVECTOR* verts;
    SVECTOR* norms;
    INDEX* indices;
    int nfaces;
    SVECTOR	rot;			/* Rotation vector for Rotmatrix */
    VECTOR	pos;	/* Translation vector for TransMatrix */
} Object;

#endif