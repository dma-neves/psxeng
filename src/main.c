#include "graphics.h"

int main()
{
	int counter = 0;
	int sec = 0;

	init();
	init_debug_font();
	
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
		
		display();
	}
	
	return 0;
}
