#!/bin/bash

. ./config.sh

#./build/linux/ReleaseStatic/Launch -elf=build/rv32/ReleaseStatic/Bare -stack=0x27FFFFF0 -t -port=0
#./build/linux/ReleaseStatic/Launch -elf=build/rv32/ReleaseStatic/Bare -stack=0x103FFFF0 -t -port=0

./build/linux/ReleaseStatic/Rv32 -headless -elf=build/rv32/ReleaseStatic/Bare -stack=0x27FFFFF0
#./build/linux/ReleaseStatic/Rv32 -elf=build/rv32/ReleaseStatic/Bare -stack=0x103FFFF0