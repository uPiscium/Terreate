# include(FetchContent)

include(fetch)
fetch_dependencies()

# message(STATUS "[Terreate] Fetching GLM...")
# FetchContent_Declare(
#     glm
#     GIT_REPOSITORY https://github.com/g-truc/glm.git
#     GIT_TAG 1.0.2
#     OVERRIDE_FIND_PACKAGE
#     GIT_PROGRESS TRUE
# )
# set(GLM_BUILD_LIBRARY ON CACHE BOOL "" FORCE) # Build as library
# set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE) # Disable tests
# FetchContent_MakeAvailable(glm)

# message(STATUS "[Terreate] Fetching spdlog...")
# FetchContent_Declare(
#     spdlog
#     GIT_REPOSITORY https://github.com/gabime/spdlog.git
#     GIT_TAG v1.16.0
#     OVERRIDE_FIND_PACKAGE
#     GIT_PROGRESS TRUE
# )
# set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE) # Disable examples
# FetchContent_MakeAvailable(spdlog)

# message(STATUS "[Terreate] Fetching SDL3...")
# FetchContent_Declare(
#     SDL3
#     GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
#     GIT_TAG release-3.2.28
#     OVERRIDE_FIND_PACKAGE
#     GIT_PROGRESS TRUE
# )
# set(SDL_SHARED OFF CACHE BOOL "" FORCE)
# set(SDL_INTERFACE ON CACHE BOOL "" FORCE)
# set(SDL_STATIC ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(SDL3)
# set(SDL3_DIR "${sdl3_BINARY_DIR}")

# message(STATUS "[Terreate] Fetching SDL_image...")
# FetchContent_Declare(
#     SDL_image
#     GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
#     GIT_TAG release-3.2.4
#     OVERRIDE_FIND_PACKAGE
#     GIT_PROGRESS TRUE
# )
# FetchContent_MakeAvailable(SDL_image)

# message(STATUS "[Terreate] Fetching SDL_mixer...")
# FetchContent_Declare(
#     SDL_mixer
#     GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
#     GIT_TAG release-2.8.1
#     OVERRIDE_FIND_PACKAGE
#     GIT_PROGRESS TRUE
# )
# FetchContent_MakeAvailable(SDL_mixer)

# message(STATUS "[Terreate] Fetching SDL_net...")
# FetchContent_Declare(
#     SDL_net
#     GIT_REPOSITORY https://github.com/libsdl-org/SDL_net.git
#     GIT_TAG release-2.2.0
#     OVERRIDE_FIND_PACKAGE
#     GIT_PROGRESS TRUE
# )
# FetchContent_MakeAvailable(SDL_net)

# message(STATUS "[Terreate] Fetching SDL_shadercross...")
# FetchContent_Declare(
#     SDL_shadercross
#     GIT_REPOSITORY https://github.com/libsdl-org/SDL_shadercross.git
#     GIT_TAG main
#     OVERRIDE_FIND_PACKAGE
#     GIT_PROGRESS TRUE
# )
# FetchContent_MakeAvailable(SDL_shadercross)

add_subdirectory(core)
add_subdirectory(graphic)
