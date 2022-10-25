/*
*	Simple psx graphics api
*	Author: David Neves
*	Based on http://lameguy64.net/tutorials/pstutorials/
*/

#include "graphics.h"

#define NTSC
//#define PAL

#define OTLEN 8 // Ordering talbe length

DISPENV disp[2];
DRAWENV draw[2];
int db;

u_long ot[2][OTLEN]; // Ordering Table with 8 elements
char primbuff[2][32768]; // Primitive buffer
char* nextPrim; // Next primitive pointer

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

	// Set initial primitive pointer address
	nextPrim = primbuff[db];
}


void init_debug_font(void)
{
	// Load internal font texture
	FntLoad(960,0);
	// Create text stream
	FntOpen(0,8,320,224,0,100);
}

void draw_rectangle(int x, int y, int width, int height, int r, int g, int b)
{
	TILE* tile;
	tile = (TILE*)nextPrim;

	// Initialize primitive
	setTile(tile);
	setXY0(tile, x, y);
	setWH(tile, width, height);
	setRGB0(tile, r, g, b);

	// Add primitive to the ordering table
	addPrim(ot[db], tile);
	nextPrim += sizeof(TILE);
}

void display(void)
{
	// Wait for GPU to finish drawing all primitives and for
	// V-Blank -> ensure consitent refresh rate
	DrawSync(0);
	VSync(0);

	PutDispEnv(&disp[db]);
	PutDrawEnv(&draw[db]);

	// Enable display
	SetDispMask(1);

	// Draw ordering table
	DrawOTag(ot[db] + OTLEN-1);

	// Switch buffers
	db = !db;

	// Set next primitive pointer to the new primitive buffer
	nextPrim = primbuff[db];
	// Clear ordering table
	ClearOTagR(ot[db], OTLEN);
}
