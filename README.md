# image-test program

This is an OpenGL C++11 test program which compiles for the web using Emscripten as well as for regular computers.

It uses [GLFW](http://www.glfw.org/) to obtain a graphics context, part of the [Boost](https://www.boost.org/) library for filesystem access on PCs, the testing framework [Catch2](https://github.com/catchorg/Catch2), [Lua](https://www.lua.org/) is used for scripting and as a configuration language (using [Fengari](https://fengari.io/) to run Lua code in the browser and [Selene](https://github.com/jeremyong/Selene) on PC), [Skia](https://skia.org/) or the HTML 5 Canvas element is used for [text texture image](https://webgl2fundamentals.org/webgl/lessons/webgl-text-texture.html) creation, and [TinyUTF8](https://github.com/DuffsDevice/tinyutf8) for Unicode strings.

## Building for Linux, Mac, and the web

There are four [makefiles](https://en.wikipedia.org/wiki/Makefile) depending on whether you're building for the web or PC, and building a testing or release build. You invoke them thus:

`makefile -f make_pc_test`

The desktop versions of the makefile will work on both Mac and Linux, but you may need to edit the hardcoded library locations (i keep all libraries in `~/github/`).

## Building for Windows

[ To be added, will use whatever version of Visual Studio Skia uses. ]

