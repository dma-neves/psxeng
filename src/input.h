/*
*	Simple psx input module
*	Author: David Neves
*	Based on http://lameguy64.net/tutorials/pstutorials/
*/

#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <sys/types.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>
#include <psxpad.h>
#include <psxapi.h>

void init_pad();
u_char button_pressed(uint16_t button_code);

#endif