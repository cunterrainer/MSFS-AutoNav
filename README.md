# AutoNav
A program to controll the auto pilot in microsoft flight simulator 2020 using a unified interface

# Build
## Prerequisites
### Microsoft flight simulator SDK
In Microsoft flight simulator go to Options &rarr; General Options &rarr; Developers &rarr; Turn Developer mode on &rarr; In the new Devmode menu select Help &rarr; SDK Installer (Core)  
Make sure to add the SDK directory to the path.  
[Visual guide](https://docs.flightsimulator.com/html/Introduction/SDK_Overview.htm)
### CMake
[Download](https://cmake.org/download/), install it and make sure to add it to your path aswell.
### Visual Studio
Install Visual Studio and the `Development with C++` Extension.

## Clone
```
git clone https://github.com/cunterrainer/MSFS-AutoNav.git
```
```
cd MSFS-AutoNav
```

## Build fltk
```
cmake -G "Visual Studio 17 2022" -A x64 -S Dependencies/fltk -B Dependencies/fltk/Build -DFLTK_BUILD_TEST=OFF -DFLTK_MSVC_RUNTIME_DLL=OFF -DFLTK_BUILD_GL=OFF
```
```
cmake --build Dependencies/fltk/Build -j --config Release
```
Use `Debug` instead of `Release` for the debug libraries

## Build AutoNav
```
vendor\premake5.exe vs2022
```
Open the Solution (.sln) and Build the desired mode (Debug or Release)