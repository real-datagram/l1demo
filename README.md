# l1demo
Code for MFP demos &amp; related tools from the [LayerOne](https://www.layerone.org/events/demo-party/) [Demo Party](http://www.l1demo.org).

This repo includes our engine code used for our [2017](https://www.youtube.com/watch?v=fUCvhj4NFEg) and [2018](https://www.youtube.com/watch?v=vhlIaRISzwA) demos.

Major features:
* Scene manager
* Dynamic resolution/BPP switching
* Scene-specific buffering (enable/disable double buffering at will)
* Customizable HSYNC/VSYNC interrupts
* Customizable pre-draw functions
* Tracked Bytebeat audio
* Build system unlocks xc16-gcc compile optimizations O[23s]

## Hardware
The demo board is based on the [PIC24FJ256DA206](https://www.microchip.com/wwwproducts/en/PIC24FJ256DA206) MCU.

The board is available for purchase on [Tindie](https://www.tindie.com/products/arkorobotics/l1-demoscene-board/).

## Requirements
These instructions are for Linux.

* [CMake](https://cmake.org/)
* [Make](https://www.gnu.org/software/make/)
* [MPLAB XC16 Compiler](https://www.microchip.com/mplab/compilers)
* [Mono](http://www.mono-project.com/download/stable/)

## Usage

### Build
```bash
make
```

The default Make target will clean, initialize the build dir, and compile.

### Program Board
Push reset on the board and it will appears as a TTYACM device for 5 seconds.

```bash
make push
```

By default the board will be programed with the ds30 loader unless `ipecmd.sh` is found.

# Demo Party Info

## Next Party

[LayerOne Demo Party](https://www.layerone.org/events/demo-party/)
May 25, 2019 @ 18:00

## Links
* [LayerOne Demo Party](http://www.l1demo.org)
* [LayerOne Conference](https://www.layerone.org)

## Past Demo Party Videos

* 2018 Party: https://www.youtube.com/watch?v=vhlIaRISzwA
* 2017 Party: https://www.youtube.com/watch?v=fUCvhj4NFEg
* 2016 Party: https://www.youtube.com/watch?v=nEnQhmWmAyQ
* 2015 Party: I can't find this video : (

# More Info

* https://github.com/arkorobotics/L1DemosceneBoard
* https://github.com/arkorobotics/L1Demo_Getting_Started

Need help getting started?
Keep eye on out for classes at [Null Space Labs](https://032.la/) on [Meetup](https://www.meetup.com/NullSpaceLabs/). (Usually a couple months before the next demo party.)
