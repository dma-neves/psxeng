#ifndef GEOMETRY_H
#define GEOMETRY_H

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

#endif