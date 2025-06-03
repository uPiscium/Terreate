#!/usr/bin/env bash
cd tests/resources/shader/ || return
glslc -o vert.spv -fshader-stage=vertex vert.glsl
glslc -o frag.spv -fshader-stage=fragment frag.glsl
cd ../../../ || return
