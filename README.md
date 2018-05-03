# image-test program

This is an OpenGL C++11 test program which compiles for the web using Emscripten as well as for regular computers using regular compilers.

It uses [GLFW](http://www.glfw.org/) to obtain a graphics context, part of the [Boost](https://www.boost.org/) library for filesystem access on desktop machines, the testing framework [Catch2](https://github.com/catchorg/Catch2), [Lua](https://www.lua.org/) for scripting and as a configuration language (using [Fengari](https://fengari.io/) to run the code in the browser and [Selene](https://github.com/jeremyong/Selene) on PC), [Skia](https://skia.org/) or the HTML 5 Canvas element is used for [text texture image](https://webgl2fundamentals.org/webgl/lessons/webgl-text-texture.html) creation, and [TinyUTF8](https://github.com/DuffsDevice/tinyutf8) for Unicode strings.

## Building for Linux, Mac, and the web

There are two [makefiles](https://en.wikipedia.org/wiki/Makefile) depending on whether you're building for the web or your actual computer, with convenience two-letter shell scripts `bw` and `bn` (for 'build web' and 'build native') which you might want to make executable and copy into somewhere your command line's PATH variable can see.

The desktop versions of the makefile will work on both Mac and Linux, but you may need to edit the hardcoded library locations (i keep all libraries in `~/github/`); these are marked with a triple star *** comment in the file.

Currently it's set to dynamically link to GLFW and GLEW, so install those via your operating system's package manager, as well as the `-devel` versions of the packages. I have no idea how do distribute Linux program binaries conveniently, and am sorta resigned to having to learn Objective-C in order to make an easily installable macOS version :(

## Building for Windows

[ To be added, will use whatever version of Visual Studio Skia uses. ]

# Code notes

All spellings are North American, not Commonwealth.

There are four global arrays indexed by enums, used to hold user-configurable settings: `::gNums`, `::gStrings`, `::gColors`, and `::gBools`.

Filenames are `CamelCase` if they define classes or structs or `c_style` if not.

There are several prefix letters used with variable names:
* `g` for global
* `m` for member
* `p` for pointer


