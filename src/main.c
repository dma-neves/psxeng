#include "graphics.h"

int main()
{
	init();
	init_debug_font();

	int counter = 0;
	int sec = 0;

	int xvel = 1;
	int yvel = 1;

	int xpos = 32;
	int ypos = 32;

	int w = 64;
	int h = 64;

    extern int tim_sonylogo64[];
    TIM_IMAGE sonylogo64;
    TimParam sonylogo64_tparam = load_texture(tim_sonylogo64, &sonylogo64);
    Rect sonylogo64_rect = (Rect){.x=xpos, .y=ypos, .width=64, .height=64, ((Color){.r=128, .g=128, .b=128}) };

	extern int tim_texture64[];
	TIM_IMAGE texture64;
	TimParam texture64_tparam = load_texture(tim_texture64, &texture64);
	Rect texture64_rect = (Rect){.x=48, .y=48, .width=64, .height=64, .color=(Color){.r=128, .g=128, .b=128}};

	Rect rect = (Rect){.x=xpos, .y=ypos, .width=64, .height=64, ((Color){.r=255, .g=255, .b=0}) };

	//set_texture_page(texture64_tparam);
	//set_texture_page(sonylogo64_tparam);

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

		// draw_rectangle( rect );

		draw_sprite(texture64_tparam, texture64_rect);
		draw_sprite(sonylogo64_tparam, sonylogo64_rect);

		if(xpos + w + xvel > 320) xvel *= -1;
		if(xpos + xvel < 0) xvel *= -1;
		if(ypos + h + xvel > 240) yvel *= -1;
		if(ypos + xvel < 0) yvel *= -1;

		xpos += xvel;
		ypos += yvel;

		sonylogo64_rect.x = xpos;
		sonylogo64_rect.y = ypos;

		display();
	}
	
	return 0;
}
