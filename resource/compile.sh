#!/usr/bin/env sh

cd resource/shader/ || exit
glslc shader.vert -o shader.vert.spv
glslc shader.frag -o shader.frag.spv
cd .. || exit
