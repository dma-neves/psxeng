/*
*	Simple psx graphics api
*	Author: David Neves
*	Based on http://lameguy64.net/tutorials/pstutorials/
*/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdio.h>
#include <sys/types.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>

void init(void);
void init_debug_font(void);
void draw_rectangle(int x, int y, int width, int height, int r, int g, int b);
void display(void);

#endif