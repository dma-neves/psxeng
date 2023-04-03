/*
*	Simple psx graphics module
*	Author: David Neves
*	Based on http://lameguy64.net/tutorials/pstutorials/
*/

#include "graphics.h"

#define NTSC
//#define PAL

#ifdef NTSC
	#define SCREEN_XRES		320
	#define SCREEN_YRES		240
#endif

#ifdef PAL
	#define SCREEN_XRES		320
	#define SCREEN_YRES		256
#endif

/* Screen center position */
#define CENTERX			SCREEN_XRES>>1
#define CENTERY			SCREEN_YRES>>1

// #define OTLEN 8 // Ordering table length
// #define PACKET_LEN 32768 // Packet length

#define OTLEN			256
#define PACKET_LEN		1024*8

/* Double buffer structure */
typedef struct {
	DISPENV	disp;			/* Display environment */
	DRAWENV	draw;			/* Drawing environment */
	u_long 	ot[OTLEN];		/* Ordering table */
	char 	prim[PACKET_LEN];	/* Packet buffer */
} DB;

/* Double buffer variables */
DB		db[2];
int		db_active = 0;
char	*next_prim;

void init_graphics(void)
{
	// aux_init();
	// return;

	ResetGraph(0); // Reset GPU and enable interrupts

	db_active = 0;

#ifdef NTSC
	// Configure display enviornments for 320*240 mode
	SetDefDispEnv(&db[0].disp, 0, 0, SCREEN_XRES, SCREEN_YRES);
	SetDefDispEnv(&db[1].disp, 0, SCREEN_YRES, SCREEN_XRES, SCREEN_YRES);

	// Configure draw enviornments for th display enviornments
	SetDefDrawEnv(&db[0].draw, 0, SCREEN_YRES, SCREEN_XRES, SCREEN_YRES);
	SetDefDrawEnv(&db[1].draw, 0, 0, SCREEN_XRES, SCREEN_YRES);

	SetVideoMode(MODE_NTSC);

#endif

#ifdef PAL
	// Configure display enviornments for 320*256 mode
	SetDefDispEnv(&db[0].disp, 0, 0, SCREEN_XRES, SCREEN_YRES);
	SetDefDispEnv(&db[1].disp, 0, SCREEN_YRES, SCREEN_XRES, SCREEN_YRES);

	// Screen offset to center the picture vertically
	db[0].disp.screen.y = 24;
	db[1].disp.screen.y = 24;

	SetVideoMode(MODE_PAL);

	// Configure draw enviornments for th display enviornments
	SetDefDrawEnv(&db[0].draw, 0, SCREEN_YRES, SCREEN_XRES, SCREEN_YRES);
	SetDefDrawEnv(&db[1].draw, 0, 0, SCREEN_XRES, SCREEN_YRES);
#endif

	// Specify clear color of the draw enviornment
	setRGB0(&db[0].draw, 0,102,102);
	setRGB0(&db[1].draw, 0,102,102);
	// Enable background clear
	db[0].draw.isbg = 1;
	db[1].draw.isbg = 1;

	// Set initial primitive pointer address
	next_prim = db[db_active].prim;

	// Apply enviornments
	PutDispEnv(&db[0].disp);
	PutDrawEnv(&db[0].draw);

	ClearOTagR(db[db_active].ot, OTLEN);
}

void init_debug_font(void)
{
	// Load internal font texture
	FntLoad(960,0);
	// Create text stream
	FntOpen(0,8,SCREEN_XRES,224,0,100);
}

void draw_rectangle(Rect rect)
{
	TILE* tile;
	tile = (TILE*)next_prim;

	// Initialize primitive
	setTile(tile);
	setXY0(tile, rect.x, rect.y);
	setWH(tile, rect.width, rect.height);
	setRGB0(tile, rect.color.r, rect.color.g, rect.color.b);

	// Add primitive to the ordering table
	addPrim(db[db_active].ot, tile);
	next_prim += sizeof(TILE);
}

void display(void)
{
	// Wait for GPU to finish drawing all primitives and for
	// V-Blank -> ensure consitent refresh rate
	DrawSync(0);
	VSync(0);

	PutDispEnv(&db[db_active].disp);
	PutDrawEnv(&db[db_active].draw);

	// Enable display
	SetDispMask(1);

	// Draw ordering table
	DrawOTag(db[db_active].ot + OTLEN-1);

	// Switch buffers
	db_active = !db_active;

	// Set next primitive pointer to the new primitive buffer
	next_prim = db[db_active].prim;
	// Clear ordering table
	ClearOTagR(db[db_active].ot, OTLEN);
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

	sprt = (SPRT*)next_prim;

	setSprt(sprt);												// Initialize the primitive (very important)
	setXY0(sprt, rect.x, rect.y);								// Position the sprite at (48,48)
	setWH(sprt, rect.width, rect.height);						// Set size to 64x64 pixels
	setUV0(sprt, tparam.uoffs, tparam.voffs); 					// Set UV coordinates
	setClut(sprt, tparam.crect.x, tparam.crect.y);				// Set CLUT coordinates to sprite
	setRGB0(sprt, rect.color.r, rect.color.g, rect.color.b); 	// Set primitive color
	addPrim(db[db_active].ot, sprt);										// Sort primitive to OT
	next_prim += sizeof(SPRT);

	DR_TPAGE* tpage = (DR_TPAGE*)next_prim;
	setDrawTPage(tpage, 0, 1, getTPage(tparam.mode&0x3, 0, tparam.prect.x, tparam.prect.y));
	addPrim(db[db_active].ot, tpage);		
	next_prim += sizeof(DR_TPAGE);
}






// ###################### 3d stuff ######################


/* Light color matrix */
/* Each column represents the color matrix of each light source and is */
/* used as material color when using gte_ncs() or multiplied by a */
/* source color when using gte_nccs(). 4096 is 1.0 in this matrix */
/* A column of zeroes disables the light source. */
MATRIX color_mtx = {
	ONE, 0, 0,	/* Red   */
	ONE, 0, 0,	/* Green */
	ONE, 0, 0	/* Blue  */
};

/* Light matrix */
/* Each row represents a vector direction of each light source. */
/* An entire row of zeroes disables the light source. */
MATRIX light_mtx = {
	/* X,  Y,  Z */
	-2048 , -2048 , -2048,
	0	  , 0	  , 0,
	0	  , 0	  , 0
};

void init_graphics_3d()
{
	/* Initialize the GTE */
	InitGeom();
	
	/* Set GTE offset (recommended method  of centering) */
	gte_SetGeomOffset( CENTERX, CENTERY );
	
	/* Set screen depth (basically FOV control, W/2 works best) */
	gte_SetGeomScreen( CENTERX );
	
	/* Set light ambient color and light color matrix */
	gte_SetBackColor( 63, 63, 63 );
	gte_SetColorMatrix( &color_mtx );
}


MATRIX	mtx,lmtx;				/* Rotation matrices for geometry and lighting */



POLY_F3	*pol3;					/* Flat shaded quad primitive pointer */

void draw_object_trig(Object* obj)
{
	int i,p,xy_temp;

	/* Set rotation and translation to the matrix */
	RotMatrix( &obj->rot, &mtx );
	TransMatrix( &mtx, &obj->pos );
	
	/* Multiply light matrix by rotation matrix so light source */
	/* won't appear relative to the model's rotation */
	MulMatrix0( &light_mtx, &mtx, &lmtx );
	
	/* Set rotation and translation matrix */
	gte_SetRotMatrix( &mtx );
	gte_SetTransMatrix( &mtx );
	
	/* Set light matrix */
	gte_SetLightMatrix( &lmtx );
	
	
	/* Draw */
	pol3 = (POLY_F3*)next_prim;	
	
	for( i=0; i<obj->nfaces; i++ ) {
		
		/* Load the first 3 vertices of a quad to the GTE */
		gte_ldv3( 
			&obj->verts[obj->indices[i].v0], 
			&obj->verts[obj->indices[i].v1], 
			&obj->verts[obj->indices[i].v2] );
			
		/* Rotation, Translation and Perspective Triple */
		gte_rtpt();
		
		/* Compute normal clip for backface culling */
		gte_nclip();
		
		/* Get result*/
		gte_stopz( &p );
		
		/* Skip this face if backfaced */
		if( p < 0 )
			continue;
		
		/* Calculate average Z for depth sorting */
		gte_avsz4();
		gte_stotz( &p );
		
		/* Skip if clipping off */
		/* (the shift right operator is to scale the depth precision) */
		if( (p>>2) > OTLEN )
			continue;
		
		/* Initialize a quad primitive */
		setPolyF3( pol3 );
		
		/* Set the projected vertices to the primitive */
		gte_stsxy0( &pol3->x0 );
		gte_stsxy1( &pol3->x1 );
		gte_stsxy2( &pol3->x2 );
		
		/* Compute the last vertex and set the result */
		// gte_ldv0( &obj->verts[obj->indices[i].v3] );
		// gte_rtps();
		// gte_stsxy( &pol3->x3 );
		
		/* Load primitive color even though gte_ncs() doesn't use it. */
		/* This is so the GTE will output a color result with the */
		/* correct primitive code. */
		gte_ldrgb( &pol3->r0 );
		
		/* Load the face normal */
		gte_ldv0( &obj->norms[i] );
		
		/* Normal Color Single */
		gte_ncs();
		
		/* Store result to the primitive */
		gte_strgb( &pol3->r0 );
		
		/* Sort primitive to the ordering table */
		addPrim( db[db_active].ot+(p>>2), pol3 );
		
		/* Advance to make another primitive */
		pol3++;
		
		if(pol3 >= &db[db_active].prim[PACKET_LEN-1])
		{
			// TODO
			break;
		}
	}
	
	/* Update nextpri variable */
	/* (IMPORTANT if you plan to sort more primitives after this) */
	next_prim = (char*)pol3;
}
