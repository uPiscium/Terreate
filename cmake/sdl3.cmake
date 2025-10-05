include(FetchContent)

find_package(SDL3 QUIET)
if(SDL3_FOUND)
  message(STATUS "[SDL3] Found installed version.")
  return()
else()
  message(STATUS "[SDL3] No installed version found, fetching...")
endif()

message(STATUS "[SDL3] Setting up...")
set(SDL_SHARED OFF)
set(SDL_INTERFACE ON)
set(SDL_STATIC ON)
FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.2.24
  OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(SDL3)
set(SDL3_DIR "${sdl3_BINARY_DIR}")
message(STATUS "[SDL3] Ready.")

