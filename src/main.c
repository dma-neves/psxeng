#include "graphics.h"

int main()
{
	int counter = 0;
	int sec = 0;

	init();
	init_debug_font();

	int xvel = 1;
	int yvel = 1;

	int xpos = 32;
	int ypos = 32;

	int w = 64;
	int h = 64;
	
	while(1)
	{
		if(counter++ >= 60)
		{
			counter = 0;
			sec++;
		}
	
		FntPrint(-1, "HELLO WORLD\n");
		FntPrint(-1, "SECONDS=%d\n", sec);
		FntFlush(-1);

		//draw_rectangle(32,32,64,64,255,255,0);
		draw_rectangle( (Rect){.x=xpos, .y=ypos, .width=64, .height=64, ((Color){.r=255, .g=255, .b=0}) } );
		draw_sprite();

		if(xpos + w + xvel > 320) xvel *= -1;
		if(xpos + xvel < 0) xvel *= -1;
		if(ypos + h + xvel > 240) yvel *= -1;
		if(ypos + xvel < 0) yvel *= -1;

		xpos += xvel;
		ypos += yvel;
		
		display();
	}
	
	return 0;
}
