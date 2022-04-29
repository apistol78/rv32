#!/bin/bash

# Get normalized path to game home, excluding file name.
GAME_HOME="`dirname \"$BASH_SOURCE\"`"
GAME_HOME="`(cd \"$GAME_HOME\" && pwd)`"
export GAME_HOME

# Setup traktor home.
. "$GAME_HOME/../traktor/scripts/config.sh"

# Add path to risc-v toolchain.
#export PATH=/opt/riscv32-imf/bin:$PATH
export PATH=/opt/riscv32/bin:$PATH