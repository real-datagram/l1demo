void initExample1(uint8_t double_buffer) {
    // scene init code here

    // config_graphics args:
    // MODE_640x480 - timing definition from vga.c
    // 640, 480 - canvas horizontal and vertical resolution
    // 1 - Color depth (bits per pixel)
    // 0 - x offset, display vs canvas offset
    // 0 - y offset, display vs canvas offset
    // double_buffer - double buffer support as passed by scene manager
    config_graphics(MODE_640x480, 640UL, 480UL, 1, 0, 0, double_buffer);

    set_clut(0, 0x0000);
    set_clut(1, 0xf100);
}

void drawExample1() {
    // frame draw code here
    
    borderScreen();
    safe_draw((HOR_RES>>1)-10, (VER_RES>>1)-10, 20, 20);
}
