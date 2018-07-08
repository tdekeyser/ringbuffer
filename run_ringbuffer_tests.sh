#!/bin/bash

# Create build folder
rm -rf build/
mkdir build/
cd build/

# Make executable
cmake ..
make
./ringbuffer_test $2
