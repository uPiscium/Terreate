root := justfile_directory()

[working-directory: root]
configure:
    #!/usr/bin/env sh
    set -eu
    if [ -f CMakePresets.json ]; then
      cmake --preset "${CMAKE_PRESET:-default}"
    else
      cmake -S . -B "${CMAKE_BUILD_DIR:-.build/default}" -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    fi

[working-directory: root]
build:
    #!/usr/bin/env sh
    set -eu
    if [ -f CMakePresets.json ]; then
      cmake --build --preset "${CMAKE_PRESET:-default}"
    else
      cmake --build "${CMAKE_BUILD_DIR:-.build/default}"
    fi
