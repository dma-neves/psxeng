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
#include <inline_c.h>

#include "geometry.h"
#include "util.h"

void init_graphics(void);
void init_debug_font(void);
void display(void);

TimParam load_texture(int* tim, TIM_IMAGE* timage);

void draw_rectangle(Rect rect);
void draw_sprite(TimParam tparam, Rect rect);

// ######################  3d stuff ######################

void init_graphics_3d();
void draw_object(Object* obj);

#endif