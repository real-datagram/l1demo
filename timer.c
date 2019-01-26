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

// Bytebeat audio functions
// Simple control of different equations in an interrupt
// Modify "track" in scenes to change audio track

void config_timer() {
    PR1 = 2000; // we are gunning for 8khz
    _T1IP = 5;  // set interrupt priority
    _TON = 1;   // turn on the timer
    _T1IF = 0;  // reset interrupt flag
    _T1IE = 0;  // turn on the timer1 interrupt
}

#define TRACK_INTRO 1
#define TRACK_EXAMPLE1 2
#define TRACK_EXAMPLE2 3
#define TRACK_RAND 255

// SOUND TRACK VARIABLES
static int32_t t=0;     // current "time"
static uint8_t track=0; // current track

void __attribute__((__interrupt__)) _T1Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void) {
    static int8_t sample=0;
    PORTB = sample << 8;

    switch(track) {
        case TRACK_INTRO:
            sample = 0; // put bytebeat here
            break;
        case TRACK_EXAMPLE1:
            sample = 0; // put bytebeat here
            break;
        case TRACK_EXAMPLE2:
            sample = 0; // put bytebeat here
            break;
        case TRACK_RAND:
            sample = rand();
            break;
        default: // 0 or *
            sample = 0; // mute
            break;
    }

    t++;
    _T1IF = 0;
}
