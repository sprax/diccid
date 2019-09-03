#!/bin/bash
# set build environment for StuffGetter

export OMPL_INSTALL_ROOT=/usr/local

if [ "$(uname)" == "Darwin" ]; then
    export OMPL_LIBRARIES=$OMPL_INSTALL_ROOT/lib/libompl.1.3.2.dylib
else
    export OMPL_LIBRARIES=$OMPL_INSTALL_ROOT/lib/libompl.1.3.2.so
fi
