#include "../../src/graphics.h"
#include "../../src/input.h"

int main()
{
	init_graphics();
	init_graphics_3d();
	init_debug_font();
	init_pad();

	extern Object car;

	int counter = 0;
	int sec = 0;

	while(1)
	{
		/* Make the cube spin */
		// car.rot.vx = 8;
		// car.rot.vz += 16;
        // car.rot.vy += 4;

		if(counter++ >= 60)
		{
			counter = 0;
			sec++;
		}
	
		FntPrint(-1, "HELLO WORLD\n");
		FntPrint(-1, "SECONDS=%d\n", sec);
		FntFlush(-1);

        if(button_pressed(PAD_RIGHT))
			car.rot.vy += 8;
		if(button_pressed(PAD_LEFT))
			car.rot.vy -= 8;
		if(button_pressed(PAD_UP))
			car.rot.vx += 8;
		if(button_pressed(PAD_DOWN))
			car.rot.vx -= 8;
        if(button_pressed(PAD_CIRCLE))
            car.rot.vz += 8;
        if(button_pressed(PAD_SQUARE))
            car.rot.vz -= 8;

		draw_object(&car);

		display();

	}
	
	return 0;
}
