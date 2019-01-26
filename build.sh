#!/bin/bash

# Compile with MFP flags - You probably don't need half this shit
# Untested on newer versions of xc16-gcc! Please let me know what's currently compatible and I'll make this build script nicer
/opt/microchip/xc16/v1.24/bin/xc16-gcc main.c -o main.o -c -mcpu=24FJ256DA206 -MMD -MF main.o.d -mno-eds-warn -omf=elf -g -mlarge-code -mlarge-data -menable-large-arrays -O1 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off && /opt/microchip/xc16/v1.24/bin/xc16-gcc -o production.elf main.o -mcpu=24FJ256DA206 -omf=elf -mlarge-arrays -Wl,--local-stack,--defsym=__MPLAB_BUILD=1,,--script=p24FJ256DA206.gld,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map=production.map,--report-mem && /opt/microchip/xc16/v1.24/bin/xc16-bin2hex production.elf -a -omf=elf && 

# Program device
/opt/microchip/mplabx/mplab_ipe/ipecmd.sh -TPPK3 -P"24FJ256DA206" -M -F`pwd`"/production.hex" -OL
