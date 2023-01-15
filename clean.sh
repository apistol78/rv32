#!/bin/bash

. ./scripts/config.sh

rm -Rf build
rm -Rf dumps
rm *.vmem
rm *.vmem-range
rm *.s16
rm *.png
rm *.fst
rm *.vcd

rm -Rf code/Rv32T/SoC
rm -Rf code/Verify/SoC
