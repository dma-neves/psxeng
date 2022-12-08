/*
*	Simple psx graphics module
*	Author: David Neves
*	Based on http://lameguy64.net/tutorials/pstutorials/
*/

#include "graphics.h"

#define NTSC
//#define PAL

#define OTLEN 8 // Ordering talbe length

/* Display and draw environments */
DISPENV disp[2];
DRAWENV draw[2];
int db;

/* Ordering table */
u_long ot[2][OTLEN]; // Ordering Table with 8 elements
char primbuff[2][32768]; // Primitive buffer
char* nextPrim; // Next primitive pointer

void init_graphics(void)
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

	// Set initial primitive pointer address
	nextPrim = primbuff[db];

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

void draw_rectangle(Rect rect)
{
	TILE* tile;
	tile = (TILE*)nextPrim;

	// Initialize primitive
	setTile(tile);
	setXY0(tile, rect.x, rect.y);
	setWH(tile, rect.width, rect.height);
	setRGB0(tile, rect.color.r, rect.color.g, rect.color.b);

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

TimParam load_texture(int* tim, TIM_IMAGE* timage)
{
	GetTimInfo(tim, timage);

	// Upload pixel data to framebuffer
	LoadImage(timage->prect, timage->paddr);
	DrawSync(0);

	// Upload CLUT to framebuffer if present

	// the color depth of the TIM file are in bits 0-3 of the mode field and 
	// CLUT presence is determined by testing bit 4. 

	if(timage->mode & 0x8)
	{
		LoadImage(timage->crect, timage->caddr);
		DrawSync(0);
	}

	// Create and return TimParam struct with all the usefull parameters

	TimParam tparam;

	// Copy the TIM coordinates
    tparam.prect = *timage->prect;
    tparam.crect   = *timage->crect;
    tparam.mode    = timage->mode;

	// Calculate U,V offset for TIMs that are not page aligned
    tparam.uoffs = (tparam.prect.x%64)<<(2-(tparam.mode&0x3));
    tparam.voffs = (tparam.prect.y&0xff);

	return tparam;
}

void draw_sprite(TimParam tparam, Rect rect)
{
	TILE *tile; // Pointer for TILE
	SPRT *sprt; // Pointer for SPR

	sprt = (SPRT*)nextPrim;

	setSprt(sprt);												// Initialize the primitive (very important)
	setXY0(sprt, rect.x, rect.y);								// Position the sprite at (48,48)
	setWH(sprt, rect.width, rect.height);						// Set size to 64x64 pixels
	setUV0(sprt, tparam.uoffs, tparam.voffs); 					// Set UV coordinates
	setClut(sprt, tparam.crect.x, tparam.crect.y);				// Set CLUT coordinates to sprite
	setRGB0(sprt, rect.color.r, rect.color.g, rect.color.b); 	// Set primitive color
	addPrim(ot[db], sprt);										// Sort primitive to OT
	nextPrim += sizeof(SPRT);

	DR_TPAGE* tpage = (DR_TPAGE*)nextPrim;
	setDrawTPage(tpage, 0, 1, getTPage(tparam.mode&0x3, 0, tparam.prect.x, tparam.prect.y));
	addPrim(ot[db], tpage);		
	nextPrim += sizeof(DR_TPAGE);
}