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

#define CHR_FGCOLOR        0x5000
#define CHR_BGCOLOR        0x5100
#define CHR_FONTBASE        0x5200
#define CHR_PRINTCHAR        0x5300
#define CHR_TXTAREASTART    0x5800
#define CHR_TXTAREAEND        0x5900
#define CHR_PRINTPOS        0x5A00

void font_color(uint16_t fg, uint16_t bg) {
    while(_CMDFUL) continue;
    G1CMDL = fg;
    G1CMDH = CHR_FGCOLOR;
    Nop();

    while(_CMDFUL) continue;
    G1CMDL = bg;
    G1CMDH = CHR_BGCOLOR;
    Nop();
}

void config_chr(void) {
    while(_CMDFUL) continue;
    G1CMDL = 0xFFFF;
    G1CMDH = CHR_FGCOLOR;
    Nop();

    while(_CMDFUL) continue;
    G1CMDL = 0x0;
    G1CMDH = CHR_BGCOLOR;
    Nop();

    while(_CMDFUL) continue;
    G1CMDL = (uint16_t)(Consolas14) & 0xFFFF; // TODO FIX THIS FOR VARIABLE FONTS
    G1CMDH = CHR_FONTBASE;
    Nop();
    
    while(_CMDFUL) continue;
    G1CMDL = 0;
    G1CMDH = CHR_TXTAREASTART;
    Nop();

    while(_CMDFUL) continue;
    G1CMDL = (HOR_RES & 0xF)<<12 | VER_RES;
    G1CMDH = CHR_TXTAREAEND | (HOR_RES >>4);
    Nop();
}

void chr_print(char *c, uint16_t txtX, uint16_t txtY) {
    while(_CMDFUL) continue;
    G1CMDL = (txtX & 0xF)<<12 | txtY;
    G1CMDH = CHR_PRINTPOS | (txtX >>4);
    Nop();

    while(*c != NULL) {
        while(_CMDFUL) continue;
        G1CMDL = *c;
        G1CMDH = CHR_PRINTCHAR;
        Nop();

        c++;
    }
}

void chr_print0(char *c)
{
    while(_CMDFUL) continue;
    G1CMDL = 0;
    G1CMDH = CHR_PRINTPOS;
    Nop();

    while(*c != NULL) {
        while(_CMDFUL) continue;
        G1CMDL = *c;
        G1CMDH = CHR_PRINTCHAR;
        Nop();

        c++;
    }
}

void chr_printchar(char c, uint16_t txtX, uint16_t txtY) {
    while(_CMDFUL) continue;
    G1CMDL = (txtX & 0xF)<<12 | txtY;
    G1CMDH = CHR_PRINTPOS | (txtX >>4);
    Nop();

    while(_CMDFUL) continue;
    G1CMDL = c;
    G1CMDH = CHR_PRINTCHAR | (1 << 7);
    Nop();

}
