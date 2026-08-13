root := justfile_directory()

[working-directory: root]
all:
    #!/usr/bin/env sh
    set -eu
    if [ -f CMakePresets.json ]; then
      ctest --preset "${CMAKE_PRESET:-default}" --output-on-failure
    else
      ctest --test-dir "${CMAKE_BUILD_DIR:-.build/default}" --output-on-failure
    fi

[working-directory: root]
unit filter:
    #!/usr/bin/env sh
    set -eu
    if [ -f CMakePresets.json ]; then
      ctest --preset "${CMAKE_PRESET:-default}" -R {{quote(filter)}} --output-on-failure
    else
      ctest --test-dir "${CMAKE_BUILD_DIR:-.build/default}" -R {{quote(filter)}} --output-on-failure
    fi
