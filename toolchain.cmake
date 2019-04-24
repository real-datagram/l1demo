if(NOT DEFINED TC_BASE_DIR)
    set(TC_BASE_DIR "/opt/microchip")
endif()

set(TC_PATH "${TC_BASE_DIR}/xc16/v${TC_VER}/bin")
set(TC_XC "xc16")

set(CMAKE_SYSTEM PIC24)
set(CMAKE_SYSTEM_PROCESSOR 24FJ256DA206)

set(CMAKE_C_COMPILER "${TC_PATH}/${TC_XC}-gcc")
set(CMAKE_C_LINKER "${TC_PATH}/${TC_XC}-ld")
set(CMAKE_AR "${TC_PATH}/${TC_XC}-ar")
set(CMAKE_OBJCOPY "${TC_PATH}/${TC_XC}-bin2hex")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
