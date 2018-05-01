# image-test program

This is an OpenGL C++11 test program which compiles for the web using Emscripten as well as for regular computers.

It uses [GLFW](http://www.glfw.org/) to obtain a graphics context, part of the [Boost](https://www.boost.org/) library for filesystem access on PCs, the testing framework [Catch2](https://github.com/catchorg/Catch2), [Lua](https://www.lua.org/) is used for scripting and as a configuration language (using [Fengari](https://fengari.io/) to run Lua code in the browser and [Selene](https://github.com/jeremyong/Selene) on PC), [Skia](https://skia.org/) or the HTML 5 Canvas element is used for [text texture image](https://webgl2fundamentals.org/webgl/lessons/webgl-text-texture.html) creation, and [TinyUTF8](https://github.com/DuffsDevice/tinyutf8) for Unicode strings.

## Building for Linux, Mac, and the web

There are two [makefiles](https://en.wikipedia.org/wiki/Makefile) depending on whether you're building for the web or PC, with convenience two-letter commands `bw` and `bn` (for 'build web' and 'build native').

The desktop versions of the makefile will work on both Mac and Linux, but you may need to edit the hardcoded library locations (i keep all libraries in `~/github/`).

Currently it's set to dynamically link to GLFW and GLEW, so install those via your operating system's package manager.

## Building for Windows

[ To be added, will use whatever version of Visual Studio Skia uses. ]

# Code notes

All spellings are North American.

There are four global variables, used to hold user-configurable settings: `::gNums`, `::gStrings`, `::gColors`, `::gBools`.

Filenames are CamelCase if they contain classes or c_style if not.

There are several prefix letters used with variable names:
* `m` for member
* `p` for pointer
* `g` for global



