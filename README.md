# Input module

## Build with MSYS2 MinGW
Install required packages
```
pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-toolchain mingw-w64-x86_64-boost cmake make gcc
```

### Build
```
cmake -B build -DCMAKE_BUILD_TYPE=<Release|Debug>
cmake --build build
```

### Install
Install artifacts: `/include`, `/lib`, `/cmake`.
```
# should use a local install location
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TEST=OFF -DCMAKE_INSTALL_PREFIX=.
cmake --build build
cmake --install build
```

## Build with MSVC
Depends on `boost`. Can use prebuilt binary for [Windows](https://sourceforge.net/projects/boost/files/boost-binaries/).

For testing, depends on [`libmatoya`](https://github.com/parsec-cloud/libmatoya) for GUI stuffs.

### Build
In `Developer Shell VS 2022`:
```
# Build libmatoya
git clone https://github.com/parsec-cloud/libmatoya
cd libmatoya
nmake ARCH=x64

# In input-module project
cmake -B build-msvc -DBOOST_ROOT=C:\boost_1_80_0 -Dlibmatoya_ROOT_DIR=/path/to/libmatoya
cmake --build build-msvc

# Run tests
.\build-msvc\tests\Debug\inputtest.exe
```

## Integrate as a subproject
```
# source code locate at: <project-root>/third-party/input-module

# in root CMakeList.txt
add_subdirectory(third-party/input-module)
target_include_directories(project-target PRIVATE third-party/input-module/include)
target_link_libraries(project-target PRIVATE inputmodule::inputmodule)
```