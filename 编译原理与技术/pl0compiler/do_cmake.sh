#!/bin/bash

if [ -d build/ ]; then
	rm -rf build
fi

mkdir build
cd build
mkdir temp
cp -r ../examples/ ./examples/ 
cp -r ../testcase/ ./testcase/
cmake ..
