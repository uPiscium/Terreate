root := justfile_directory()

[working-directory: root]
format-check:
    #!/usr/bin/env sh
    set -eu
    files=$(git ls-files '*.h' '*.hh' '*.hpp' '*.c' '*.cc' '*.cpp' '*.cxx')
    [ -z "$files" ] || clang-format --dry-run --Werror $files

[working-directory: root]
lint:
    #!/usr/bin/env sh
    set -eu
    files=$(git ls-files '*.cc' '*.cpp' '*.cxx')
    [ -z "$files" ] || clang-tidy -p "${CMAKE_BUILD_DIR:-.build/default}" $files
