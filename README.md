# Silicon
A realtime platform for creating interactive media. It targets Windows, macOS, Linux, and the Web (via WebAssembly).

## Requirements
Silicon includes the source (and licences to redistribute the source) of its dependencies in-tree. This means that you need nothing more than:
- A C++17 Compiler
- CMake
  - Version 3.24 or newer if building for Desktop
  - Version 3.16 or newer if building for Web
- Your favourite build tool (Whichever one CMake supports)
- Emscripten (Only if building for the Web)

## Generating

> #### Dependencies
> Make sure to run `git submodule update --init --recursive` to download and update all dependencies

### For Desktop

> #### Boost 
> Silicon Desktop depends on Boost, which needs some pre-building steps itself. Silicon provides scripts to assist with this. Run `scripts/setup.sh` (macOS, Linux only), or `scripts/setup.ps1` (Any supported platform with PowerShell 7 or newer installed) from Silicon's root directory.
```shell
cmake -G <your preferred tool> -B <your preferred folder> .
```

```shell
cmake -G <your preferred tool> -B <your preferred folder> -DSI_PLATFORM=Desktop . # Optional
```
### For Web
```shell
cmake -G <your preferred tool> -B <your preferred folder> -DSI_PLATFORM=Web . # Required
```

## Building
```shell
cmake --build <your preferred folder>
```

## Linking
Silicon is designed to be used as a static library. In other words, you call Silicon as demonstrated in [test/EngineInit/EngineInit.cpp](test/EngineInit/EngineInit.cpp):
```cpp
#include <cstdlib>

#include "Silicon/Silicon.hpp"

int main(int argc, char** argv)
{
    if (Si::Initialize())
    {
        Si::Deinitialize();
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
```
If your project uses CMake, you are encouraged to submodule this repository and use `add_subdirectory()`.