# image-test program

This is an OpenGL C++11 test program which compiles for the web using Emscripten as well as for regular computers using regular compilers.

## Prerequisites

To run the program natively i *think* you need [GLFW3](http://www.glfw.org/) installed.

To build it you need the development versions of:
* [GLFW](http://www.glfw.org/) to obtain a graphics context
* parts of the [Boost](https://www.boost.org/) library for filesystem access in the non-web version
* [Lua](https://www.lua.org/) for scripting and as a configuration language 
* [Skia](https://skia.org/) for 2D graphics in the native code version (the web version uses the HTML 5 Canvas element); these are used for [text texture image](https://webgl2fundamentals.org/webgl/lessons/webgl-text-texture.html) creation

For completeness, the other projects this program makes use of are:
* the testing framework [Catch2](https://github.com/catchorg/Catch2)
* [TinyUTF8](https://github.com/DuffsDevice/tinyutf8) for Unicode handling
* [Fengari](https://fengari.io/) to run Lua code in the browser and [Selene](https://github.com/jeremyong/Selene) to talk to native Lua from C++

..but the code for these is already included in this repository.

## Building for Linux, Mac, and the web

There are two [makefiles](https://en.wikipedia.org/wiki/Makefile) depending on whether you're building for the web or your actual computer, with convenience invocation scripts `bw` and `bn` and `rn` and `bnr` (for 'build web', 'build native executable', 'run native', and 'build and then run native') which you might want to make executable and copy into somewhere visible to your command line's `PATH` variable.

The desktop versions of the makefile will work on both Mac and Linux, but you may need to edit the hardcoded library locations (i had difficulties geting `pkg-config` to find GLFW in the Mac); these are marked with a triple star *** comment in the file.

Currently it's set to dynamically link to GLFW and GLEW, so install those via your operating system's package manager, as well as the `-devel` versions of the packages. I have no idea how to distribute Linux program binaries conveniently, and am sorta resigned to having to learn Objective-C in order to make an easily installable macOS version :(

## Building for Windows

[ To be added, will use whatever version of Visual Studio Skia uses. ]

# Code notes

All spellings are North American, but measurements are in centimetres.

There are four global arrays indexed by enums, used to hold user-configurable settings: `::gNums` (for floats), `::gStrings`, `::gColors`, and `::gBools`. `The global.h` file also contains debugging and convenience macros. A global is also used for the program's single App object as i couldn't figure out how to pass arguments to Emscripten's main loop function. Another global contains the pixel memory that text is drawn onto before being sent to the graphics card.

There's a neat trick for embedding GLSL code files inside your compiled binaries near the bottom of `GlContext.cpp`.

Filenames are `CamelCase` if they define classes or structs or `c_style` if not.

Variable name prefixes:
* `my` for class member variables
* `g` for global
* `p` for pointer

## Things to come back to

Switch out `glTexImage3D` for the more efficient `glTexStorage3D` [once that is supported by Emscripten](https://github.com/kripken/emscripten/issues/5747) (it's funny Emscripten only supports the older version, because i also found [a thread about how the WebGL working group had been controversially planning to drop support for it and only keep the newer one in the WebGL spec](https://www.khronos.org/webgl/public-mailing-list/public_webgl/1410/msg00033.php)).

The Linux version can't display emoji, unlike the Mac and Web ones. May need to load a special font? https://groups.google.com/forum/#!topic/skia-discuss/1uJoMAvUXXo https://forums.xamarin.com/discussion/82233/how-to-use-mutiple-ttf-file-of-typeface-in-skiasharp or glyph layout library https://stackoverflow.com/questions/30757193/find-out-if-character-in-string-is-emoji?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa



