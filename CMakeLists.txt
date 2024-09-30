cmake_minimum_required(VERSION 3.28)
include(FetchContent)

project(LvglCmakeExample LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for clang-tidy

set(TARGET_PLATFORM sdl2 CACHE STRING "Target platform")
set_property(CACHE TARGET_PLATFORM PROPERTY STRINGS sdl2)

add_subdirectory(src/platforms/${TARGET_PLATFORM})

if(TARGET_PLATFORM STREQUAL "sdl2")
  list(APPEND LV_LINK_LIBRARIES_PRIVATE SDL2::SDL2)
endif()

# This defines the generated lvgl config header.
# The actual command to produce the header files from `defconfig` is defined further down in this file.
add_custom_target(kconfig_header_file DEPENDS "${CMAKE_BINARY_DIR}/generated_lvgl_conf/generated_lvgl_conf.h")
add_library(kconfig_header INTERFACE)
add_dependencies(kconfig_header kconfig_header_file)
target_include_directories(kconfig_header SYSTEM INTERFACE "${CMAKE_BINARY_DIR}/generated_lvgl_conf")
target_compile_definitions(kconfig_header INTERFACE LV_CONF_KCONFIG_EXTERNAL_INCLUDE=<generated_lvgl_conf.h>)

# We add the generated header target as a PUBLIC lvgl dependency, so that its include path
# is available to LVGL users.
list(APPEND LV_LINK_LIBRARIES_PUBLIC kconfig_header)

# We use Kconfig-based defconfig rather than a manual lvconf.h, so we must set `LV_CONF_SKIP`.
set(LV_CONF_SKIP ON)
add_subdirectory(src/third_party/lvgl)

# Finally, we define the command that generates the header.
# This must be done after the `add_subdirectory(src/third_party/lvgl)` call because
# the commmand relies on `lvgl_SOURCE_DIR` (set by the `FetchContent_MakeAvailable(lvgl)` call in `src/third_party/lvgl`).
make_directory("${CMAKE_BINARY_DIR}/generated_lvgl_conf")
add_custom_command(
  OUTPUT "${CMAKE_BINARY_DIR}/generated_lvgl_conf/generated_lvgl_conf.h"
  COMMAND
  env KCONFIG_CONFIG='${CMAKE_SOURCE_DIR}/src/platforms/${TARGET_PLATFORM}/lvgl_defconfig'
  ${CMAKE_SOURCE_DIR}/scripts/genconfig.py
  --header-path='${CMAKE_BINARY_DIR}/generated_lvgl_conf/generated_lvgl_conf.h'
  '${lvgl_SOURCE_DIR}/Kconfig'
  DEPENDS "src/platforms/${TARGET_PLATFORM}/lvgl_defconfig" "${lvgl_SOURCE_DIR}/Kconfig"
)

add_executable(lvgl-cmake-example
  src/platforms/${TARGET_PLATFORM}/main.cpp
  src/app_main.cpp
)
target_link_libraries(lvgl-cmake-example PRIVATE
  lvgl::lvgl
  ${TARGET_PLATFORM}_platform
  kconfig_header
)
