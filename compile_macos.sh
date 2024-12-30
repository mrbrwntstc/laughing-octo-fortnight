#!/bin/bash

PROJECT_DIR=$(pwd)
BUILD_DIR="${PROJECT_DIR}/build"
EXE_NAME="linear_algebra"

SRC_FILES="${PROJECT_DIR}/main.cpp"

# Step 2: Create the build directory
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory..."
    mkdir "$BUILD_DIR"
fi

clang++ -g -std=c++11 -o "$BUILD_DIR/$EXE_NAME" $SRC_FILES