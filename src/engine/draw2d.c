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

// GPU safe color set
void safe_color(uint16_t color) {
    while(_CMDFUL) continue;
    G1CMDL = color;
    G1CMDH = RCC_COLOR;
    Nop();
}

// GPU unsafe color set
void fast_color(uint16_t color) {
    //while(_CMDFUL) continue;
    G1CMDL = color;
    G1CMDH = RCC_COLOR;
    //Nop();
}


// GPU unsafe draw with raster operation configurable
void rcc_draw(uint16_t oper, uint16_t rop, uint16_t x, uint16_t y, uint16_t w, uint16_t h) {

    // destination
    //while(_CMDFUL) continue;
    G1CMDL = x + y * HOR_RES;
    G1CMDH = RCC_DESTADDR | ((uint32_t)y * HOR_RES + x) >> 16;
    Nop();
    Nop();
    //while(!_CMDMPT) continue;

    // size
    //while(_CMDFUL) continue;
    G1CMDL = (w<<12) | h;
    G1CMDH = RCC_RECTSIZE | (w>>4);
    Nop();
    Nop();
    //while(!_CMDMPT) continue;

    // go!
    //while(_CMDFUL) continue;
    G1CMDL = rop<<3 | oper<<7;
    G1CMDH = RCC_STARTCOPY;
    Nop();
    Nop();
    //while(!_CMDMPT) continue;
}

// GPU safe draw operations
// Raster operations hardcoded for speed - use rcc_draw for other ops
void safe_draw(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {

    //uint16_t oper = 0x0;
    //uint16_t rop = 0xc;

    // destination
    while(_CMDFUL) continue;
    G1CMDL = x + y * HOR_RES;
    G1CMDH = RCC_DESTADDR | ((uint32_t)y * HOR_RES + x) >> 16;
    Nop();
    Nop();
    while(!_CMDMPT) continue;

    // size
    while(_CMDFUL) continue;
    G1CMDL = (w<<12) | h;
    G1CMDH = RCC_RECTSIZE | (w>>4);
    Nop();
    Nop();
    while(!_CMDMPT) continue;

    // go!
    while(_CMDFUL) continue;
    G1CMDL = 0x60; //rop<<3 | oper<<7;
    G1CMDH = RCC_STARTCOPY;
    Nop();
    Nop();
    while(!_CMDMPT) continue;
}

void fast_draw(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {

    //uint16_t oper = 0x0;
    //uint16_t rop = 0xc;

    // destination
    //while(_CMDFUL) continue;
    G1CMDL = x + y * HOR_RES;
    G1CMDH = RCC_DESTADDR | ((uint32_t)y * HOR_RES + x) >> 16;
    //Nop();
    //Nop();
    //while(!_CMDMPT) continue;

    // size
    //while(_CMDFUL) continue;
    G1CMDL = (w<<12) | h;
    G1CMDH = RCC_RECTSIZE | (w>>4);
    //Nop();
    //Nop();
    //while(!_CMDMPT) continue;

    // go!
    //while(_CMDFUL) continue;
    G1CMDL = 0x60; //rop<<3 | oper<<7;
    G1CMDH = RCC_STARTCOPY;
    //Nop();
    //Nop();
    //while(!_CMDMPT) continue;
}

// "faster" version for single pixel draws
// required? TODO
// WILL NOT ACCOUNT FOR TRUE PIXEL HEIGHT
void fast_pdraw(uint16_t x, uint16_t y) {
    G1CMDL = x + y * HOR_RES;
    G1CMDH = RCC_DESTADDR | ((uint32_t)y * HOR_RES + x) >> 16;

    G1CMDL = 4097; //(w<<12) | h;
    G1CMDH = RCC_RECTSIZE;// | (w>>4);

    G1CMDL = 0x60; //rop<<3 | oper<<7;
    G1CMDH = RCC_STARTCOPY;
}

// Raw 8bpp single pixel draw to draw buffer
// You probably don't want to use this
void raw_draw(uint16_t x, uint16_t y, uint8_t index) {
    // 8BPP only currently
    DrawBuffer[y*HOR_RES + x] = index;
}

// Wipe screen with black
// raster op 0 'black' in most cases
void clearScreen()
{
    rcc_draw(0,0, 0,0, HOR_RES, VER_RES);

    ////alternate color example
    //safe_color(0x0);
    //rcc_draw(0, 0xC, 0, 0, HOR_RES, VER_RES);
}

// Border screen with highest color / white
// You can change the boundaries a little depending on resolution
// and timing accuracy for last column (can't go off the edge)
void borderScreen()
{
    safe_color(0xff);
    safe_draw(0,0, 1, VER_RES); // left
    safe_draw(HOR_RES-2,0,1,VER_RES); // right
    safe_draw(0,0, HOR_RES-2, 1); // top
    safe_draw(0,VER_RES-1,HOR_RES-2,1); // bottom

}

// Mix 16 bit color from floats with alpha support
uint16_t mix(float r, float g, float b, float a)
{
    uint16_t res=0;
    res += (uint16_t)(r * a * 0x1F) << 11;
    res += (uint16_t)(g * a * 0x3F) << 5;
    res += (uint16_t)(b * a * 0x1F);
    return res;
}


// Double buffering support function
// Don't modify unless you know what you're doing!
void rcc_srcdest(__eds__ uint8_t *src, __eds__ uint8_t *dest)
{
    while(!_CMDMPT) continue;
    G1W1ADRL = (unsigned long)(src) & 0xFFFF;
    G1W2ADRL = (unsigned long)(dest) & 0xFFFF;
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Float and 32 bit pre-calculation tables used for sin/cos lookups
// and fixed-point operations. Because these take up a lot of global
// memory we re-use them pretty heavily for as-needed storage per-scene
// because we're too stupid to think up something better.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

float fsin_angleX[360];
int32_t isin_angleX[360];

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// You can speed these up a lot by making no modulus versions
// but you'd need to ensure you don't go out of bounds!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// Floating Point Sine & Cosine Lookup Functions
float fsin(uint16_t i) {
    return (float)fsin_angleX[i%360];
}

float fcos(uint16_t i) {
    return (float)fsin_angleX[(i+90)%360];
}

// Integral Sine & Cosine Lookup Functions
int32_t isin(uint16_t i) {
    return isin_angleX[i%360];
}

int32_t icos(uint16_t i) {
    return isin_angleX[(i+90)%360];
}
