#!/bin/bash
# 第一次使用前须在终端运行 chmod +x build.sh
set -e

BUILD_DIR="build"
BIN_DIR="bin"

BUILD_TYPE=${1:-Debug}  # 默认Debug，可传Release参数切换

echo "Build type: $BUILD_TYPE"

if [ ! -d "$BUILD_DIR" ]; then
    mkdir -p "$BUILD_DIR"
fi

if [ ! -d "$BIN_DIR" ]; then
    mkdir -p "$BIN_DIR"
fi

echo ">>> Running cmake..."
cd "$BUILD_DIR"
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
cd ..
echo "<<< Cmake completed"

echo ">>> Building project..."
cd "$BUILD_DIR"
make
cd ..
echo "<<< Build completed"
