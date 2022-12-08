/*
*	Simple psx input module
*	Author: David Neves
*	Based on http://lameguy64.net/tutorials/pstutorials/
*/

#include "input.h"

/*
Button codes already defined inside psxpad.h

#define PAD_SELECT      1
#define PAD_L3          2
#define PAD_R3          4
#define PAD_START       8
#define PAD_UP          16
#define PAD_RIGHT       32
#define PAD_DOWN        64
#define PAD_LEFT        128
#define PAD_L2          256
#define PAD_R2          512
#define PAD_L1          1024
#define PAD_R1          2048
#define PAD_TRIANGLE    4096
#define PAD_CIRCLE      8192
#define PAD_CROSS       16384
#define PAD_SQUARE      32768
*/

u_char padbuff[2][34];

void init_pad()
{
    InitPAD(padbuff[0], 34, padbuff[1], 34);

    padbuff[0][0] = padbuff[0][1] = 0xff;
    padbuff[1][0] = padbuff[1][1] = 0xff;

    StartPAD();
    ChangeClearPAD(1);
}

u_char button_pressed(uint16_t button_code)
{
    PADTYPE* pad = (PADTYPE*)padbuff[0];

    if(pad->stat == 0)
    {
        // Only parse when a digital pad, dual-analog or dual-shock is connected
        if( ( pad->type == 0x4 ) || ( pad->type == 0x5 ) || ( pad->type == 0x7 ) )
        {
            return !(pad->btn & button_code);
        }
    }
    
    return 0;
}