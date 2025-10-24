function(prepare_sdl3)
  find_package(SDL3 QUIET)
  if(SDL3_FOUND)
    message(STATUS "[SYSTEM] Found installed SDL3.")
    return()
  else()
    message(STATUS "[SYSTEM] No installed SDL3 found, fetching...")
  endif()

  message(STATUS "[SYSTEM] Setting up...")
  set(SDL_SHARED OFF)
  set(SDL_INTERFACE ON)
  set(SDL_STATIC ON)

  include(FetchContent)
  FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.24
    OVERRIDE_FIND_PACKAGE
  )
  FetchContent_MakeAvailable(SDL3)
  set(SDL3_DIR "${sdl3_BINARY_DIR}")
  message(STATUS "[SYSTEM] SDL3 Ready.")
endfunction()
