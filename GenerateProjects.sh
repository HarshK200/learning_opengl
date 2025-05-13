#!/bin/bash

echo "Generating project files using gamke"
./vendor/bin/premake/premake5 gmake

echo "Building with Bear to generate compile_commands.json..."
bear -- make -j$(nproc)
