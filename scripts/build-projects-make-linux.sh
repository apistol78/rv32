#!/bin/bash

# Source environment configuration.
. "./scripts/config.sh"

# Generator configuration.
export MAKE_OS=linux
export MAKE_DIALECT=make
export MAKE_FORMAT=$TRAKTOR_HOME/resources/build/configurations/make-format.sbm
export MAKE_INCLUDE="\$(TRAKTOR_HOME)/resources/build/configurations/make-config-linux.inc"

# Optional parameters.
export MAKE_TOOLCHAIN=gcc
export MAKE_OBJECT_FILE="%s.o"
export MAKE_STATIC_LIBRARY_FILE="lib%s.a"
export MAKE_SHARED_LIBRARY_FILE="lib%s.so"
export MAKE_EXECUTABLE_FILE="%s"
export MAKE_VERBOSE=false

# Generate build files.
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.SolutionBuilder.App \
	-f=make2 \
	-make-solution-template=$TRAKTOR_HOME/resources/build/configurations/make-solution.sbm \
	-make-project-template=$TRAKTOR_HOME/resources/build/configurations/make-project.sbm \
	Rv32Linux.xms \
	$1
