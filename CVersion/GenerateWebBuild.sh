#!/bin/bash
git submodule update --init --recursive
mkdir -p build_web
cd build_web
cmake .. -DEMSCRIPTEN=True
