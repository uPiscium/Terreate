include(FetchContent)

function(fetch_repo name git_repo git_tag)
  message(STATUS "[Terreate] Fetching ${name}...")
  FetchContent_Declare(
      ${name}
      GIT_REPOSITORY ${git_repo}
      GIT_TAG ${git_tag}
      OVERRIDE_FIND_PACKAGE
      GIT_PROGRESS TRUE
  )
  FetchContent_MakeAvailable(${name})
  message(STATUS "[Terreate] ${name} fetched.")
endfunction()

function(fetch_glm)
  set(GLM_BUILD_LIBRARY ON CACHE BOOL "" FORCE) # Build as library
  set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE) # Disable tests
  fetch_repo("glm" "https://github.com/g-truc/glm.git" "1.0.2")
endfunction()

function(fetch_spdlog)
  set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE) # Disable examples
  fetch_repo("spdlog" "https://github.com/gabime/spdlog.git" "v1.16.0")
endfunction()

function(fetch_sdl3)
  set(SDL_SHARED OFF CACHE BOOL "" FORCE)
  set(SDL_INTERFACE ON CACHE BOOL "" FORCE)
  set(SDL_STATIC ON CACHE BOOL "" FORCE)
  fetch_repo("SDL3" "https://github.com/libsdl-org/SDL.git" "release-3.2.28")
  set(SDL3_DIR "${sdl3_BINARY_DIR}")
endfunction()

function(fetch_sdl_image)
  fetch_repo("SDL_image" "https://github.com/libsdl-org/SDL_image.git" "release-3.2.4")
endfunction()

function(fetch_sdl_mixer)
  fetch_repo("SDL_mixer" "https://github.com/libsdl-org/SDL_mixer.git" "release-2.8.1")
endfunction()

function(fetch_sdl_net)
  fetch_repo("SDL_net" "https://github.com/libsdl-org/SDL_net.git" "release-2.2.0")
endfunction()

function(fetch_sdl_shadercross)
  fetch_repo("SDL_shadercross" "https://github.com/libsdl-org/SDL_shadercross.git" "main")
endfunction()

function(fetch_dependencies)
  fetch_glm()
  fetch_spdlog()
  fetch_sdl3()
  # fetch_sdl_image()
  # fetch_sdl_mixer()
  # fetch_sdl_net()
  # fetch_sdl_shadercross()
endfunction()
