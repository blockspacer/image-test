# image-test program

This is an OpenGL C++11 test program which compiles for the web using Emscripten as well as for regular computers using regular compilers.

## Prerequisites

To run the program natively i *think* you need [GLFW3](http://www.glfw.org/) installed.

To build it you need the development versions of:
* [GLFW](http://www.glfw.org/) to obtain a graphics context
* parts of the [Boost](https://www.boost.org/) library for filesystem access on desktop machines
* [Lua](https://www.lua.org/) for scripting and as a configuration language 
* [Skia](https://skia.org/) for 2D graphics in the native code version (the web version uses the HTML 5 Canvas element); these are used for [text texture image](https://webgl2fundamentals.org/webgl/lessons/webgl-text-texture.html) creation

For completeness, the other projects this program makes use of are:
* the testing framework [Catch2](https://github.com/catchorg/Catch2)
* [TinyUTF8](https://github.com/DuffsDevice/tinyutf8) for Unicode handling
* [Fengari](https://fengari.io/) to run Lua code in the browser and [Selene](https://github.com/jeremyong/Selene) to talk to native Lua from C++
..but the code for these is already included in this repository.

## Building for Linux, Mac, and the web

There are two [makefiles](https://en.wikipedia.org/wiki/Makefile) depending on whether you're building for the web or your actual computer, with convenience two-letter shell scripts `bw` and `bn` (for 'build web' and 'build native') which you might want to make executable and copy into somewhere visible to your command line's `PATH` variable.

The desktop versions of the makefile will work on both Mac and Linux, but you may need to edit the hardcoded library locations (i had difficulties geting `pkg-config` to find GLFW in the Mac); these are marked with a triple star *** comment in the file.

Currently it's set to dynamically link to GLFW and GLEW, so install those via your operating system's package manager, as well as the `-devel` versions of the packages. I have no idea how to distribute Linux program binaries conveniently, and am sorta resigned to having to learn Objective-C in order to make an easily installable macOS version :(

## Building for Windows

[ To be added, will use whatever version of Visual Studio Skia uses. ]

# Code notes

All spellings are North American, not Commonwealth.

There are four global arrays indexed by enums, used to hold user-configurable settings: `::gNums` (for doubles), `::gStrings`, `::gColors`, and `::gBools`.

Filenames are `CamelCase` if they define classes or structs or `c_style` if not.

There are several prefix letters used with variable names:
* `g` for global
* `m` for member of the class currently being defined
* `p` for pointer


