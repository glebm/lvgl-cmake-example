add_subdirectory(third_party/sdl2)

add_library(sdl2_platform INTERFACE)
target_link_libraries(sdl2_platform INTERFACE
  SDL2::SDL2
  SDL2::SDL2main
)
