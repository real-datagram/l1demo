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

// GPU Safe Clut Change
static inline void set_clut(uint16_t index, uint16_t color)
{
    G1CLUT = 0x8100;//0b1000000100000000;
	G1CLUTbits.CLUTADR = index; // Color Look-up Table Memory Address
	G1CLUTbits.CLUTRWEN = 1; // Color Look-up Table Read/Write Enable
	G1CLUTWR = color; // Color Look-up Table Memory Write Data

    while(!_CMDMPT) continue;

    while(G1CLUTbits.CLUTBUSY);// Read the CLUTBUSY status bit and check
	G1CLUTbits.CLUTRWEN = 0;
}

// GPU unsafe CLUT change - you probably don't want to use this
static inline void fast_clut(uint16_t index, uint16_t color)
{
    G1CLUT = 0x8100;//0b1000000100000000;
	G1CLUTbits.CLUTADR = index; // Color Look-up Table Memory Address
	G1CLUTbits.CLUTRWEN = 1; // Color Look-up Table Read/Write Enable
	G1CLUTWR = color; // Color Look-up Table Memory Write Data

	G1CLUTbits.CLUTRWEN = 0;
}
