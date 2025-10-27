include(FetchContent)

message(STATUS "[Terreate] Setting up FreeType...")
FetchContent_Declare(
  freetype
  GIT_REPOSITORY https://github.com/freetype/freetype.git
  GIT_TAG VER-2-13-3
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(freetype)
message(STATUS "[Terreate] FreeType ready.")

