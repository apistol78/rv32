#!/bin/bash

. ./config.sh

./build/linux/ReleaseStatic/Launch -upload=Bare.hex -stack=0x27FFFFF0 -t -port=0
#./build/linux/ReleaseStatic/Launch -upload=Bare.hex -stack=0x103FFFF0 -t -port=0

#./build/linux/ReleaseStatic/Rv32 -hex=Bare.hex -stack=0x27FFFFF0
#./build/linux/ReleaseStatic/Rv32 -hex=Bare.hex -stack=0x103FFFF0
