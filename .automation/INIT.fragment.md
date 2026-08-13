## C++/CMake Project Adapter initialization

- `just project::doctor` requires CMake, Ninja, a C++ compiler, clang-format, clang-tidy, and CTest.
- `CMakeLists.txt` must exist.
- When `CMakePresets.json` exists, `CMAKE_PRESET` selects the configure/build/test preset; it defaults to `default`.
- Generated build output must remain inside the current worktree. The supplied preset uses `.build/default`.
- Do not infer or silently change compiler, generator, toolchain file, public ABI, or dependency resolution policy.
- Clean/delete operations are not part of the automatically allowed Project API.
