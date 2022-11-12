# Input module

## Build
Release
```
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TEST=OFF
cmake --build build
```

Debug and Tests
```
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Install
Install artifacts: `/include`, `/lib`, `/cmake`.
```
# should use a local install location
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TEST=OFF -DCMAKE_INSTALL_PREFIX=.
cmake --build build
cmake --install build
```

## Integrate as a subproject
```
# source code locate at: <project-root>/third-party/input-module

# in root CMakeList.txt
add_subdirectory(third-party/input-module)
target_include_directories(project-target PRIVATE third-party/input-module/include)
target_link_libraries(project-target PRIVATE inputmodule::inputmodule)
```