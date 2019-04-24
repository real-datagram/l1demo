.PHONY: init build clean all push push-bootloader push-ipe
.NOTPARALLEL:
.POSIX:
.ONESHELL:
.DEFAULT_GOAL = all

BUILD_DIR ?= build
CMAKE_C_FLAGS ?= -Os
PROGRAMER ?=
PROJECT_NAME ?= l1demo
TC_BASE_DIR ?= /opt/microchip
TC_VER ?= $(shell find "${TC_BASE_DIR}" -type f -name xc16-gcc | sort | head -1 | grep -P 'v\d\.\d\d/bin/' | sed 's|^.*v\([[:digit:]]\.[[:digit:]][[:digit:]]\)/.*|\1|')

init:
	mkdir -p ${BUILD_DIR}
	cmake -DTC_VER="${TC_VER}" -DTC_BASE_DIR="${TC_BASE_DIR}" -DPROJECT_NAME="${PROJECT_NAME}" -DCMAKE_C_FLAGS="${CMAKE_C_FLAGS}" -B "${BUILD_DIR}" -G "CodeBlocks - Unix Makefiles" "."

build:
	cmake --build ${BUILD_DIR}

clean:
	rm -rf -- ${BUILD_DIR} bin

all: clean init build

push:
	if  [ "${PROGRAMER}" == "DS30LOADER" ]; then \
	  $(MAKE) push-ds30loader
	elif [ "${PROGRAMER}" == "IPE" ] || [ -x "${TC_BASE_DIR}/mplabx/mplab_ipe/ipecmd.sh" ]; then \
	  $(MAKE) push-ipe
	else \
	  $(MAKE) push-ds30loader
	fi

push-ds30loader:
	$(eval $@_DS30L_PATH = tools/ds30Loader/bin/ds30LoaderConsole.exe)
	if [ ! -x "$($@_DS30L_PATH)" ]; then \
	  git submodule --init --remote --recommend-shallow --force --checkout -- tools/ds30Loader
	fi
	$(eval $@_TTY = $(shell find  /dev -name ttyACM\* | grep -F ACM | tail -1))
	mono "$($@_DS30L_PATH)" -f="bin/${PROJECT_NAME}.hex" -r=115200 -d=pic24fj256da206 -k=$($@_TTY) --writef -o

push-ipe:
	${TC_BASE_DIR}/mplabx/mplab_ipe/ipecmd.sh -TPPK3 -P"24FJ256DA206" -M -F"bin/${PROJECT_NAME}.hex" -OL
