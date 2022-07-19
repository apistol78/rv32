#!/bin/bash

. ./config.sh

#./build/linux/ReleaseStatic/Launch -memcheck -memcheck-from=0x10000000 -memcheck-to=0x10000100
#./build/linux/ReleaseStatic/Launch -memcheck -memcheck-from=0x20110000 -memcheck-to=0x20111000
#./build/linux/ReleaseStatic/Launch -memcheck -memcheck-from=0x5a000000 -memcheck-to=0x5a001000

#./build/linux/ReleaseStatic/Launch -elf=build/rv32/ReleaseStatic/Doom -stack=0x20FFFFF0 -t -port=0
./build/linux/ReleaseStatic/Launch -elf=build/rv32/ReleaseStatic/Doom -t -port=1

#./build/linux/ReleaseStatic/Rv32 -elf=build/rv32/ReleaseStatic/Bare -stack=0x20FFFFF0
