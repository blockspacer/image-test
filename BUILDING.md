Linux and mac: run `make -f makefile_for_native_release` from the `src/` directory. 

## Building the Skia graphics library on Arch Linux

This works on commit 0cc01b753ac6750e8decbcb92e4e33de1b147e20, dated 2018-05-10, but it's the same process as a year ago when i first build Skia, i just forgot to keep notes that time.

Edit file `github/skia/gn/is_clang.py` to read simply `print('false')`

Remove `"-Werror",`, line 97 of `~/github/skia/gn/BUILD.gn`

Run `~/github/skia/bin/gn gen out/Static --args="is_official_build=true skia_use_libpng=false skia_use_libjpeg_turbo=false skia_use_libwebp=false"`

The file `subsetter_impl.cc` in the directory `~/github/skia/third_party/externals/sfntly/cpp/src/sample/chromium/` needs to have this include inserted:

```c++
#include "../../../../../icu/source/common/unicode/unistr.h"
```

Run `ninja -C out/Static`


