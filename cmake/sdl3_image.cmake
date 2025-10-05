include(FetchContent)

message(STATUS "[SDL3_image] Setting up...")
set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(
  SDL3_image
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
  GIT_TAG release-3.2.4
  OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(SDL3_image)
message(STATUS "[SDL3_image] Ready.")
