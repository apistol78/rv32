#!/bin/bash

. ./config.sh

#./build/linux/ReleaseStatic/Launch -elf=build/rv32/ReleaseStatic/Doom -stack=0x27FFFFF0 -t -port=0
./build/linux/ReleaseStatic/Launch -elf=build/rv32/ReleaseStatic/Doom -stack=0x1000fff0 -t -port=0

#./build/linux/ReleaseStatic/Rv32 -elf=build/rv32/ReleaseStatic/Bare -stack=0x27FFFFF0
