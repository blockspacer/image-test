# image-test program

This is an OpenGL C++11 test program which compiles for the web using Emscripten and WebGL 2 (identical to OpenGL ES 3) as well as for regular computers.

It uses the [Boost](https://www.boost.org/) library for filesystem access on PCs, 
(The testing framework used is [Catch2](https://github.com/catchorg/Catch2).)


## Building for Linux, Mac, and the web

There are four [makefiles](https://en.wikipedia.org/wiki/Makefile) depending on whether you're building for the web or PC, and building a test or release build. You invoke them thus:

`makefile -f make_pc_test`


