void initExample2(uint8_t double_buffer) {
    // scene init code here
    config_graphics(MODE_640x480, 640UL, 480UL, 1, 0, 0, double_buffer);

    set_clut(0, 0x0000);
    set_clut(1, 0x001f);
}

void drawExample2() {
    // frame draw code here
    
    borderScreen();
    safe_draw((HOR_RES>>1)-40, (VER_RES>>1)-10, 20, 20);
    safe_draw((HOR_RES>>1)+40, (VER_RES>>1)-10, 20, 20);
}
