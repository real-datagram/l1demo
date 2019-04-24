cmake_minimum_required(VERSION 3.13)

if(NOT DEFINED PROJECT_NAME)
    set(PROJECT_NAME "l1demo")
endif()
project(${PROJECT_NAME} C)

if(NOT DEFINED TC_VER)
    set(TC_VER "1.36")
endif()

# Easier to use include than CMAKE_TOOLCHAIN_FILE.
include(toolchain.cmake)

set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/bin")

if(NOT DEFINED CMAKE_C_FLAGS)
    set(CMAKE_C_FLAGS "-Os")
endif()
# Undocumented license flag: -mafrlcsj
# see https://hackaday.io/project/27250-mcu-how-tos-reviews-rants/log/72734-exploring-microchip-xc16-compiler
set(CMAKE_C_FLAGS "-mcpu=${CMAKE_SYSTEM_PROCESSOR} -omf=elf -MMD -msmart-io=1 -Wall -msfr-warn=off -mno-eds-warn -mlarge-code -mlarge-data -menable-large-arrays -funroll-loops -mafrlcsj ${CMAKE_C_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "-mlarge-arrays -Wl,--local-stack,--defsym=__MPLAB_BUILD=1,,--script=p24FJ256DA206.gld,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map=${PROJECT_NAME}.map,--report-mem")

add_executable(${PROJECT_NAME}.elf
        src/main.c
        )

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND ${CMAKE_OBJCOPY}
        ARGS ${PROJECT_NAME}.elf -a -omf=elf
        COMMENT "Objcopy: bin to hex"
        )

target_include_directories(${PROJECT_NAME}.elf PUBLIC
        include
        src
        src/engine
        ${TC_PATH}/../support/generic/h/
        )
