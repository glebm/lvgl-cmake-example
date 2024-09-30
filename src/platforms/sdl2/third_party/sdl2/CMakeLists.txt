FetchContent_Declare(SDL2
  URL https://github.com/libsdl-org/SDL/archive/refs/tags/release-2.30.7.tar.gz
  URL_HASH MD5=ace1a2fcfd1001690471f2e9a04b3ddc
  EXCLUDE_FROM_ALL
  FIND_PACKAGE_ARGS GLOBAL)
FetchContent_MakeAvailable(SDL2)
