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


// VGA configuration and GPU initialization functions
// IT IS STRONGLY ADVISED TO NOT FUCK WITH THIS FILE UNLESS YOU KNOW WHAT YOU ARE DOING

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <xc.h>
#include <string.h>

#define  FCY    16000000UL    // Instruction cycle frequency, Hz
#include <libpic30.h>

_CONFIG1(FWDTEN_OFF & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_HS & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & FCKSM_CSDCMD)
_CONFIG3(ALTPMP_ALTPMPEN & SOSCSEL_EC)

#define  MODE_80x480 0
#define MODE_160x480 1
#define MODE_320x480 2
#define MODE_640x480 3
#define MODE_640x480_TESTING 4
#define MODE_800x600 5

uint16_t VER_TRICK_OFFSET = 0;
uint16_t HOR_TRICK_OFFSET = 0;
uint16_t HOR_RES = 0; //160UL;
uint16_t VER_RES = 0; //240UL;
uint16_t HOR_REAL = 0;//160UL;
uint16_t VER_REAL = 0;//480UL;
uint16_t BPP = 8UL;

uint16_t HOR_FRONT_PORCH = 32;
uint16_t HOR_PULSE_WIDTH = 8;
uint16_t HOR_BACK_PORCH = 32;
uint16_t VER_FRONT_PORCH = 10;
uint16_t VER_PULSE_WIDTH = 5;
uint16_t VER_BACK_PORCH = 10;
uint16_t VER_FUDGE = 0;
uint16_t HOR_FUDGE = 0;

#define GFX_BUFFER_SIZE ((640UL * 480UL) / (8/1))

int fb = 0;

__eds__ uint8_t GFXDisplayBuffer[GFX_BUFFER_SIZE * 2] __attribute__((eds));
__eds__ uint8_t *DisplayBuffer = GFXDisplayBuffer;
__eds__ uint8_t *DrawBuffer = GFXDisplayBuffer+GFX_BUFFER_SIZE; 

#define RCC_SRCADDR             0x6200
#define RCC_DESTADDR            0x6300
#define RCC_RECTSIZE            0x6400
#define RCC_COLOR               0x6600
#define RCC_STARTCOPY           0x6700

void config_graphics(uint16_t mode, uint16_t resX, uint16_t resY, uint16_t bpp, uint16_t offX, uint16_t offY, uint8_t buffer_mode) {
    HOR_RES = resX;
    VER_RES = resY;
    HOR_TRICK_OFFSET = offX;
    VER_TRICK_OFFSET = offY;         
    BPP = bpp;

    if (buffer_mode == 1) { // single buffered
        __eds__ uint8_t *DisplayBuffer = GFXDisplayBuffer;
        __eds__ uint8_t *DrawBuffer = GFXDisplayBuffer;
    } else { // double buffered
        __eds__ uint8_t *DisplayBuffer = GFXDisplayBuffer;
        //__eds__ uint8_t *DrawBuffer = GFXDrawBuffer; 
        __eds__ uint8_t *DrawBuffer = &GFXDisplayBuffer+GFX_BUFFER_SIZE; 
    }
    
    if (mode == MODE_80x480) {
        HOR_REAL = 80UL;
        VER_REAL = 480UL;

        HOR_FRONT_PORCH = 32;
        HOR_PULSE_WIDTH = 8;
        HOR_BACK_PORCH = 32;
        VER_FRONT_PORCH = 10;
        VER_PULSE_WIDTH = 5;
        VER_BACK_PORCH = 10;
        HOR_FUDGE = 2;
        VER_FUDGE = 1;

        _G1CLKSEL = 1;
        _GCLKDIV = 69;
    }
    else if (mode == MODE_160x480)
    {
        HOR_REAL = 160UL;
        VER_REAL = 480UL;
    
        HOR_FRONT_PORCH = 32;
        HOR_PULSE_WIDTH = 24;
        HOR_BACK_PORCH = 32;
        VER_FRONT_PORCH = 10;
        VER_PULSE_WIDTH = 5;
        VER_BACK_PORCH = 10;
        if (resY != VER_REAL) {
            HOR_FUDGE = 1;
            VER_FUDGE = 0;
        } else {
            HOR_FUDGE = 2;
            VER_FUDGE = 1;
        }
        
        _G1CLKSEL = 1;
        _GCLKDIV = 46;
    }
    else if (mode == MODE_320x480)
    {
        HOR_REAL = 320UL;
        VER_REAL = 480UL;

        HOR_FRONT_PORCH = 32;
        HOR_PULSE_WIDTH = 24;
        HOR_BACK_PORCH = 32;
        VER_FRONT_PORCH = 10;
        VER_PULSE_WIDTH = 5;
        VER_BACK_PORCH = 10;
        if (resY != VER_REAL) {
            HOR_FUDGE = 1;
            VER_FUDGE = 0;
        } else {
            HOR_FUDGE = 2;
            VER_FUDGE = 1;
        }

        _G1CLKSEL = 1;
        _GCLKDIV = 24; // 24 or 26 no edge artifacts but uncentered

    }
    else if (mode == MODE_640x480)
    {
        HOR_REAL = 640UL;
        VER_REAL = 480UL;
    
        HOR_FRONT_PORCH = 16;
        HOR_PULSE_WIDTH = 96;
        HOR_BACK_PORCH = 48;
        VER_FRONT_PORCH = 10;
        VER_PULSE_WIDTH = 2;
        VER_BACK_PORCH = 0;
        HOR_FUDGE = 1;
        VER_FUDGE = 33;

        _G1CLKSEL = 1;
        _GCLKDIV = 12; // 11 works but has artifacts
    }
    else if (mode == MODE_640x480_TESTING)
    {
        HOR_REAL = 640UL;
        VER_REAL = 480UL;

        // technically industry standard timings are used here and not the other 640x480
        // due to how VER_BACK_PORCH is interpretted
        HOR_FRONT_PORCH = 16;
        HOR_PULSE_WIDTH = 96;
        HOR_BACK_PORCH = 48;
        VER_FRONT_PORCH = 10;
        VER_PULSE_WIDTH = 2;
        VER_BACK_PORCH = 33;
        HOR_FUDGE = 0;
        VER_FUDGE = 0;

        _G1CLKSEL = 1;
        _GCLKDIV = 11; // 11 works but has artifacts, 12 works but seems wrong in some scenarios
    } else if (mode == MODE_800x600)
    {
        HOR_REAL = 800UL;
        VER_REAL = 600UL;

        // You're not ready for this res, Arko.
        HOR_FRONT_PORCH = 16;
        HOR_PULSE_WIDTH = 96;
        HOR_BACK_PORCH = 48;
        VER_FRONT_PORCH = 40;
        VER_PULSE_WIDTH = 2;
        VER_BACK_PORCH = 33;
        HOR_FUDGE = 1;
        VER_FUDGE = 1;

        _G1CLKSEL = 1;
        _GCLKDIV = 8;
    }
    
    _CPDIV = 0;

    _PUW = _DPW = _PUW = HOR_RES; // Work area and FB size so GPU works
    _PUH = _DPH = _PUH = VER_RES;   

    _DPMODE = 1;      /* TFT */
    _GDBEN = 0xFFFF;

    _DPWT = HOR_FRONT_PORCH + HOR_PULSE_WIDTH + HOR_BACK_PORCH + HOR_REAL + HOR_FUDGE;
    _DPHT = VER_FRONT_PORCH + VER_PULSE_WIDTH + VER_BACK_PORCH + VER_REAL + VER_FUDGE;
    _DPCLKPOL = 0;
    _DPENOE = 0;
    _DPENPOL = 0;
    _DPVSOE = 1;      /* use VSYNC */
    _DPHSOE = 1;      /* use HSYNC */
    _DPVSPOL = 0;     /* VSYNC negative polarity */
    _DPHSPOL = 0;     /* HSYNC negative polarity */
    _ACTLINE = _VENST = VER_FRONT_PORCH + VER_PULSE_WIDTH + VER_BACK_PORCH + VER_TRICK_OFFSET;
    _ACTPIX = _HENST = HOR_FRONT_PORCH + HOR_PULSE_WIDTH + HOR_BACK_PORCH + HOR_TRICK_OFFSET;
    _VSST = VER_FRONT_PORCH;
    _HSST = HOR_FRONT_PORCH;
    _VSLEN = VER_PULSE_WIDTH;
    _HSLEN = HOR_PULSE_WIDTH;
    _DPPWROE = 0;
    _DPPINOE = 1;
    _DPPOWER = 1;

    if (BPP == 1) _DPBPP = _PUBPP = 0;
    else if (BPP == 2) _DPBPP = _PUBPP = 1;
    else if (BPP == 4) _DPBPP = _PUBPP = 2;
    else if (BPP == 8) _DPBPP = _PUBPP = 3;
    else if (BPP == 16) _DPBPP = _PUBPP = 4;
    
    _G1EN = 1;
    __delay_ms(1);
}

void doNothing(){}
void (*hsyncFunction)() = &doNothing;
void (*vsyncFunction)() = &doNothing;
uint16_t currLine = 0;

// Vsync/Hsync parent interrupt handler
void __attribute__((interrupt, no_auto_psv))_GFX1Interrupt(void)
{

    if(_VMRGNIF)
    {
        vsyncFunction();
        currLine = 0;    
        _VMRGNIF = 0; 
       
    }
    else if(_HMRGNIF)
    {
        hsyncFunction();
        currLine++;
        _HMRGNIF = 0;
    }
    
    _GFX1IF = 0;
}

