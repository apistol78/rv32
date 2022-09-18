#!/bin/bash

. ./config.sh

#./build/linux/ReleaseStatic/Launch -memcheck -memcheck-from=0x10000000 -memcheck-to=0x10000100
#./build/linux/ReleaseStatic/Launch  -memcheck -memcheck-from=0x20020000 -memcheck-to=0x20020000
#./build/linux/ReleaseStatic/Launch -memcheck -memcheck-from=0x5a000000 -memcheck-to=0x5a001000

./build/linux/ReleaseStatic/Launch -elf=build/rv32/ReleaseStatic/Boot -t -reset
#./build/linux/ReleaseStatic/Launch -elf=build/rv32/ReleaseStatic/Doom -t -reset
#./build/linux/ReleaseStatic/Launch -elf=build/rv32/ReleaseStatic/ScummRV -t -reset
#./build/linux/ReleaseStatic/Launch -elf=build/rv32/ReleaseStatic/Quake -t -reset
