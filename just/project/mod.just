root := justfile_directory()
bootstrap_script := root / ".automation/bin/project_bootstrap.py"

mod cmake 'cmake.just'
mod quality 'quality.just'
mod tests 'test.just'
mod? repository 'repository.just'

[working-directory: root]
bootstrap name='':
    python3 {{quote(bootstrap_script)}} {{quote(name)}}

[working-directory: root]
doctor:
    @test -f CMakeLists.txt
    @command -v cmake >/dev/null
    @command -v ninja >/dev/null
    @compiler="${CXX:-c++}"; command -v "$compiler" >/dev/null
    @command -v clang-format >/dev/null
    @command -v clang-tidy >/dev/null
    @command -v ctest >/dev/null
    @if [ -f CMakePresets.json ]; then cmake --list-presets >/dev/null; fi
    @echo "C++/CMake Project Adapter doctor: PASS"

configure: cmake::configure

format-check: quality::format-check

lint: configure quality::lint

build: configure cmake::build

test: build tests::all

check: doctor format-check lint test
    @echo "C++/CMake Project Adapter check: PASS"
