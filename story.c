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

__eds__ uint8_t *swap; // double buffering temporary pointer

struct Clip
{
    void (*initPtr)();      // scene init function (before anything is drawn)
    void (*drawPtr)();      // scene drawing function
    void (*preDrawPtr)();   // pre-scene drawing function (between draws / before 1st draw)
    void (*hSyncPtr)();     // h-sync interrupt function
    void (*vSyncPtr)();     // v-sync interrupt function
    uint16_t numFrames;     // length of scene in frames (literal time varies based on frame draw time)
    uint8_t flags;          // scene flags
                            // 8|7|6|5|4|3|2|1
                            // -|-|-|-|-|-|-|(double buffering - 0: double buffer, 1: single buffer)
};

#define DBUF 0
#define SBUF 1

struct Clip story[20];   // scene instances, adjust as needed to save the bytes
uint16_t storyFrame = 0; // scene frame counter
uint16_t storyScene = 0; // scene counter

// Scene Logic Includes
// Moved down so scenes have global access to storyFrame and storyScene
#include "scene_dummy.c"
#include "scene_example1.c"
#include "scene_example2.c"

void setupStory()
{
    // Pre load lookup tables so the first scene doesn't have to do it
    // Modify as needed for float vs fixed point / integer tables
    int16_t d;
    for (d = 0; d < 360; d++) {
        fsin_angleX[d] = sin((float)d * 3.14f / 180.0f);
    }

    // Pre load sync functions to null function
    hsyncFunction = &doNothing;
    vsyncFunction = &doNothing;
    uint8_t max_scenes=0;

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // Resolution switching and scenes
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // MFP demos run on specific displays that enables fast resolution swapping without major interruption of
    // the VGA signal (AKA a projector that allows this - Epson EX3212). Using different resolutions on a
    // standard monitor will result in a) monitor stops showing images, b) monitor stops for a second or two
    // to do "signal searching", c) resolution swapping works but monitor auto-centering is broken and puts
    // scenes at seemingly random offsets of the screen OR constantly re-adjusts placement.
    //
    // Our advice is to send a 640x480 signal in the first dummy and disable "auto" settings on the display to
    // prevent VGA signal interruption between scenes of different resolutions. Note that you may need to
    // provide different config_graphics offsets for different displays to center non-standard resolutions.
    //
    // If your whole demo is the same resolution/BPP you will probably not run into these issues!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // DO NOT REMOVE THE DUMMY! BREAKS RES SWITCHING!
    story[max_scenes++] = (struct Clip) {initDummy, drawDummy, clearScreen, doNothing, doNothing, 100, DBUF};
    // PLACE A DUMMY BETWEEN SCENES WITH DIFFERENT RESOLUTIONS, BUFFERING, AND CLEARING
    // MUST PUT 2 FRAME DUMMY WITH preDrawPtr clearScreen BEFORE doNothing SCENES OR YOU'LL GET ARTIFACTS

    // Add your scenes here, don't forget dummy between scenes for res swapping tricks!
    
    story[max_scenes++] = (struct Clip) {initExample1, drawExample1, clearScreen, doNothing, doNothing, 2000, DBUF};
    story[max_scenes++] = (struct Clip) {initDummy, drawDummy, clearScreen, doNothing, doNothing, 2, DBUF};
    story[max_scenes++] = (struct Clip) {initExample2, drawExample2, clearScreen, doNothing, doNothing, 2000, DBUF};

}

void drawStory()
{
    if (storyFrame >= story[storyScene].numFrames)
    {
        storyFrame = 0;
        storyScene++;
        // set new scene sync function pointers
        vsyncFunction = story[storyScene].vSyncPtr;
        hsyncFunction = story[storyScene].hSyncPtr;
        // call new scene init
        story[storyScene].initPtr(story[storyScene].flags&1);
    }
  
    // set scene buffering 
    if (!(story[storyScene].flags&1)) { 
        rcc_srcdest(DrawBuffer, DrawBuffer);
    } else {
        __eds__ uint8_t *DisplayBuffer = GFXDisplayBuffer; 
        rcc_srcdest(DisplayBuffer, DisplayBuffer);
    }

    // call scene pre-draw function
    story[storyScene].preDrawPtr();
   
    // call scene draw function, single frame 
    // note: if frame takes too long to render you will lose FPS
    story[storyScene].drawPtr(storyFrame);
   
    // increment scene frame counter 
    storyFrame++;
    
    while(!_CMDMPT) continue;
    // swap draw and display buffers (for double buffered scenes)    
    if (!(story[storyScene].flags&1)) {  
        swap = DisplayBuffer;
        DisplayBuffer = DrawBuffer;
        DrawBuffer = swap;
    }
    G1DPADRL = (unsigned long)(DisplayBuffer) & 0xFFFF;
    G1DPADRH = 0;

    _VMRGNIF = 0;
    while (!_VMRGNIF){}
}
