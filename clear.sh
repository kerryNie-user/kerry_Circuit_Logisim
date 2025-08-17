#!/bin/bash
# 第一次使用前须在终端运行 chmod +x clear.sh
set -e

BUILD_DIR="build"
BIN_DIR="bin"
WORKSPACE_OUTPUT="output"

rm -rf $BUILD_DIR
rm -rf $BIN_DIR
rm -rf $WORKSPACE_OUTPUT
rm -rf .vscode

clear