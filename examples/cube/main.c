#include "../../src/graphics.h"
#include "../../src/input.h"

int main()
{
	init_graphics();
	init_graphics_3d();
	init_debug_font();
	init_pad();

	extern Object cube;

	int counter = 0;
	int sec = 0;

	while(1)
	{
		/* Make the cube spin */
		cube.rot.vx += 16;
		cube.rot.vz += 16;

		if(counter++ >= 60)
		{
			counter = 0;
			sec++;
		}
	
		FntPrint(-1, "HELLO WORLD\n");
		FntPrint(-1, "SECONDS=%d\n", sec);
		FntFlush(-1);

		draw_object_trig(&cube);

		display();

	}
	
	return 0;
}
