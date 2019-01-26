/***************************************************
      .____           _____          ____. 
      |   _|   ______/ ____\_____   |_   | 
      |  |    /     \   __\\____ \    |  | 
      |  |   |  Y Y  \  |  |  |_> >   |  | 
      |  |_  |__|_|  /__|  |   __/   _|  | 
      |____|       \/      |__|     |____| 

       MFP Demo Engine v1.0 (Jan 2019)
   For the LayerOne Demo Party PIC24F Board

   Code by datagram, jkthecjer, pusslord2k6

Please email datagram@layerone.org with any bugs.

***************************************************/

#include "vga.c"
#include "asset_fonts.h"
#include "chr.c"
#include "timer.c"
#include "clut.c"
#include "draw2d.c"
//#include "draw3d.c" // just kidding
                      // OR ARE WE
#include "story.c"

int main(void)
{
    //OSCCON = 0x0000;
    ANSB = 0x0000;
    ANSC = 0x0000;
    ANSD = 0x0000;
    ANSF = 0x0000;
    ANSG = 0x0000;
    TRISB = 0x0000;

    G1DPADRL = (unsigned long)(GFXDisplayBuffer) & 0xFFFF;
	G1DPADRH = 0; //(unsigned long)(GFXDisplayBuffer) >>16 & 0xFF;
	G1W1ADRL = (unsigned long)(GFXDisplayBuffer) & 0xFFFF;
	G1W1ADRH = 0; //(unsigned long)(GFXDisplayBuffer) >>16 & 0xFF;
	G1W2ADRL = (unsigned long)(GFXDisplayBuffer) & 0xFFFF;
    G1W2ADRH = 0; //(unsigned long)(GFXDisplayBuffer) >>16 & 0xFF;

    config_graphics(MODE_640x480, 640UL, 480UL, 1UL, 0, 0, 0); // Enable Graphics
    config_chr(); // Enable Character GPU
	config_timer(); // Enable Sound Interrupt

    // GFX Interrupts
	_VMRGNIF = 0; // Vsync interrupt state
	_VMRGNIE = 1; // Vsync interrupt enable
	_HMRGNIF = 0; // Hsync interrupt state
    _HMRGNIE = 0; // Hsync interrupt enable
    _GFX1IE = 0;  // Parent GFX interrupt enable (see vga.c)

    setupStory();
    
    while (1)
    {
        drawStory();
    }

    return 0;
}
