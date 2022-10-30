/*
*	Simple psx graphics module
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

#include "geometry.h"

void init(void);
void init_debug_font(void);
void draw_rectangle(Rect rect);
void display(void);
void load_assets(); 
void draw_sprite();

#endif