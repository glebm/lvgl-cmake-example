# lvgl CMake example

This is an example project using lvgl + CMake with `defconfig`-based configuration,
and an SDL2 target platform.

1. lvgl is set up via `FetchContent` in `src/third_party/lvgl/CMakeLists.txt`
2. lvgl is configured for the target platform via `defconfig`, e.g. `src/platforms/sdl2/lvgl_defconfig`.
3. The lvgl config header is generated from `lvgl_defconfig` using https://github.com/ulfalizer/Kconfiglib.

## Dependencies

Ubuntu / Debian:

```sh
sudo apt-get install -y cmake
```

## Build for PC (SDL)

Build and run in debug mode:

```sh
cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
build/lvgl-cmake-example
```
