function(set_build_mode)
  if(TERREATE_DEBUG_BUILD)
    message(STATUS "[Terreate] Building with debug flags.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra -fsanitize=address,undefined -fno-omit-frame-pointer")
    add_compile_definitions(TERREATE_DEBUG_BUILD)
  else()
    message(STATUS "[Terreate] Building with release flags.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")
  endif()
endfunction()

function(setup_include target)
  target_include_directories(${target} PUBLIC ./include)
  target_include_directories(${target} PUBLIC ../../include)
  target_include_directories(${target} PUBLIC ../../deps)
endfunction()

function(setup_link target)
  find_package(Vulkan REQUIRED)
  target_link_libraries(
    ${target} PRIVATE
    Vulkan::Vulkan
    SDL3::SDL3
    TerreateCommonModule
    TerreateSDLModule
    TerreateVulkanModule
  )
endfunction()

function(setup_binary_dir target)
  set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                   ${CMAKE_BINARY_DIR}/bin)
endfunction()
