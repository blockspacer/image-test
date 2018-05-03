# Some notes on writing C++ code that runs inside browsers using Emscripten

To **call C++ code from JavaScript**, you need to:

* wrap the function you want to call in an `extern "C" { }` block if you happen to be using the `-O0` zero optimizations flag for speedier recompiles.
* pass the name of the function to the compiler using `-s EXPORTED_FUNCTIONS = ["function_names_go_here", "another one"]` on the command line, otherwise it might get deadcode eliminated cos the compiler can't see it being used anywhere in the C++ code. You can also do `-s EXPORTED_FUNCTIONS = @filename.txt` where the file contains the same list of function names.

To **call JavaScript from C++** use the `EM_ASM( { javascript_code(); }, args... )` macro. The arguments are mandatory since i'm trying to keep spurious compiler warnings to zero so i can see when something important really does show up, so i just pass 0 if there would otherwise be no args.

If you want to **compile your C++ as native code as well** as for in-browser use, wrap browser specific bits in preprocessor directive blocks like

`#ifdef __EMSCRIPTEN__`

`    // browser code here`

`#else`

`    // native version here`

`#endif`


Again with the **zero spurious compiler warnings**, this // from https://stackoverflow.com/questions/3378560/how-to-disable-gcc-warnings-for-a-few-lines-of-code is a funky thing to easily temporarily mute warnings you know are unnecessary.




