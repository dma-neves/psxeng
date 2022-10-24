/*
*	Simple psx graphics api
*	Author: David Neves
*	Based on http://lameguy64.net/tutorials/pstutorials/
*/

#include "graphics.h"

#define NTSC
//#define PAL

DISPENV disp[2];
DRAWENV draw[2];
int db;

void init(void)
{
	ResetGraph(0); // Reset GPU and enable interrupts

	db = 0;

#ifdef NTSC
	// Configure display enviornments for 320*240 mode
	SetDefDispEnv(&disp[0], 0, 0, 320, 240);
	SetDefDispEnv(&disp[1], 0, 240, 320, 240);

	// Configure draw enviornments for th display enviornments
	SetDefDrawEnv(&draw[0], 0, 240, 320, 240);
	SetDefDrawEnv(&draw[1], 0, 0, 320, 240);
#endif

#ifdef PAL
	// Configure display enviornments for 320*256 mode
	SetDefDispEnv(&disp[0], 0, 0, 320, 256);
	SetDefDispEnv(&disp[1], 0, 256, 320, 256);

	// Screen offset to center the picture vertically
	disp[0].screen.y = 24;
	disp[1].screen.y = 24;

	SetVideoMode(MODE_PAL);

	// Configure draw enviornments for th display enviornments
	SetDefDrawEnv(&draw[0], 0, 256, 320, 256);
	SetDefDrawEnv(&draw[1], 0, 0, 320, 256);
#endif

	// Specify clear color of the draw enviornment
	setRGB0(&draw[0], 0,102,102);
	setRGB0(&draw[1], 0,102,102);
	// Enable background clear
	draw[0].isbg = 1;
	draw[1].isbg = 1;

	// Apply enviornments
	PutDispEnv(&disp[0]);
	PutDrawEnv(&draw[0]);
}


void init_debug_font(void)
{
	// Load internal font texture
	FntLoad(960,0);
	// Create text stream
	FntOpen(0,8,320,224,0,100);
}

void display(void)
{
	// Wait for GPU to finish drawing all primitives and for
	// V-Blank -> ensure consitent refresh rate
	DrawSync(0);
	VSync(0);

	// Switch buffers
	db = !db;
	PutDispEnv(&disp[db]);
	PutDrawEnv(&draw[db]);

	// Enable display
	SetDispMask(1);
}
