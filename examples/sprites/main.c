#include "../../src/graphics.h"
#include "../../src/input.h"

int main()
{
	init_graphics();
	init_debug_font();
	init_pad();

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

		if(button_pressed(PAD_RIGHT))
			texture64_rect.x++;
		if(button_pressed(PAD_LEFT))
			texture64_rect.x--;
		if(button_pressed(PAD_UP))
			texture64_rect.y--;
		if(button_pressed(PAD_DOWN))
			texture64_rect.y++;

		display();

	}
	
	return 0;
}
