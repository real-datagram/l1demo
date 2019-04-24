
void initDummy(uint8_t double_buffer)
{
    config_graphics(MODE_640x480, 640UL, 480UL, 1UL, 0UL, 0UL, double_buffer);
    hsyncFunction = &doNothing;
    vsyncFunction = &doNothing;
    _HMRGNIE = 0;
    //_VMRGNIE = 0;
    _GFX1IE = 0;
}

void drawDummy(uint16_t storyFrame)
{
    //borderScreen();
}
