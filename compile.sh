#!/bin/bash
# 第一次使用前须在终端运行 chmod +x compile.sh
set -e

BIN_DIR="bin"
BIN_OUTPUT="bin/output"
WORKSPACE_OUTPUT="output"

rm -rf bin/res
cp -r res bin/res

echo ">>> Running executable..."
cd "$BIN_DIR"
./Logisim
cd ..
echo "<<< Executable completed"

if [ -d "$WORKSPACE_OUTPUT" ]; then
    rm -rf "$WORKSPACE_OUTPUT"
fi

if [ -d "$BIN_OUTPUT" ]; then
    mv "$BIN_OUTPUT" "$WORKSPACE_OUTPUT"
fi
