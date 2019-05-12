Software Setup
=========================================

1. Download MFP Demo Engine (v1.0 as of 03/2019)
* https://github.com/real-datagram/l1demo

2. Download & install MPLAB X IDE (v5.15 as of 03/2019)
* https://www.microchip.com/mplab/mplab-x-ide
* "Downloads" tab

3. Download & install Microchip XC16 Compiler (v1.36b as of 03/2019)
* https://www.microchip.com/mplab/compilers
* "Downloads" tab (make sure you get XC16, not the other ones!)

4. Download & Install Mono (Linux/Mac only)
* http://www.mono-project.com/download/stable/

5. Download & Install L1 Demo Board Bootloader Drivers (Windows only)
* (64 bit systems) http://l1demo.org/driver/Demox64.EXE
* (32 bit systems) http://l1demo.org/driver/Demox86.EXE

6. Download L1 Demo Gui Loader
* https://github.com/thedeltaflyer/L1Demo-GUI-Loader/releases/

Compiling the MFP Engine Example Demo
=========================================

If you'd like to write code in a text editor/without a GUI, please see build.sh for the commands used for the XC16 GCC compiler.

Otherwise, we recommend using MPLAB X IDE. Please read the following instructions *carefully* and make sure you don't miss anything. If you get to the end and it doesn't compile it is likely because you missed a step. We strongly recommend starting a NEW PROJECT and going through these steps again if this happens.

1. Get your code in MPLAB X IDE

`Start a new project: File->New Project

>>> Standalone Project

>>> Family: 16-bit MCUs (PIC24)

>>> Device: PIC24FJ256DA206

>>> Tool (none / push Next)

>>> Select Compiler: XC16 (should be green) 

>>> Project Name: My Sweet Demo`


2. Add engine source code to your project:

`Right click "Source Files->Add Existing Items..."

>>> Add all of the .c and .h files from the MFP Engine to your project`


3. Change MPLAB to only focus on main.c for compilation:
(Feel free to set this up like a real programmer would as extra credit)

`File->Project Properties

>>> File Inclusion/Exclusion

Included files: main.c

Excluded files: *all others*`


4. Change compilation flags to accomodate engine specifics:

`File->Project Properties

>>> xc16-gcc -> Additional options:

Add: -mlarge-code -mlarge-data -menable-large-arrays -funroll-loops -O1`


5. Compile!

* Production->Build Main Project OR push F11 OR click the hammer icon on the toolbar

You should see "BUILD SUCCESSFUL" if everything went well. If not, you'll have to review the compilation info to see what went wrong.

Your resulting files are, by default, in:
* (Windows) c:\*user_name*\MPLABXProjects\*project_name*\dist\default\production\
* (Linux) /opt/microchip/*something similar to windows version?*

* production.hex (hex formatted file; used by bootloader/programmers)
* production.elf (binary/ELF version)

Building with USB Bootloader/GUI Tool
=========================================

Once your demo is compiled, you can flash it onto the demo board. The hex file produced earlier is what gets flashed to the board.

Make sure you installed/downloaded:
* Mono (Linux/Mac only)
* Windows drivers (Windows only)
* L1 Demo GUI Loader

To program your board with the USB Bootloader, select the .hex file and:

1. Press the RESET button on the L1 Demoscene Board
2. On the GUI, under the “Device” drop down, select the USB COM/tty port for the L1 Demoscene Board
3. Press the RESET button on the L1 Demoscene Board again
4. Click the “PROGRAM” button on the GUI

You have 5 seconds between pushing reset and selecting the GUI options/clicking buttons. If you're too slow just push reset and start again.  See http://l1demo.org/wiki/demoboard_getting_started for additional info.

Building for Linux/Command Line
=========================================

*** PICKIT3 PROGRAMMING ONLY ***

If you have a pickit3 you can program directly on the command line using MPLAB's "IPE" tool. The MFP engine includes a build.sh script which calls xc16-gcc\* and mplab_ipe directly and program with the pickit3 programmer\*\*. You can either install the versions identified in the script or update the paths for newer versions of the tools. You might need to modify the script for newer versions; sometimes parameters change.

The build script probably works on Windows/Mac (besides changing the paths) but has only been tested on Linux with the versions of software identified in the build script.

You can also flash the board with a pickit3 via the MPLAB X IDE GUI, but we (MFP team) don't use this. To do this, change your Project settings to include the pickit3 Tool and then use the "Make and Program Device Main Project" button.

\*: Removing some of the compiler flags _will_ break the MFP engine! Feel free to customize your build script as you see fit, but remember that it can affect how the engine works and may cause compilation errors!

\*\*: If not obvious, flashing your board with the pickit3 will overwrite the default USB bootloader that comes with the board.

Additional Resources
=========================================

Official "Getting Started" page
* http://l1demo.org/wiki/demoboard_getting_started

Includes additional details about board hardware and instructions for using the USB Bootloader software.
