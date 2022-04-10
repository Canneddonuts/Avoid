#!/bin/sh
emcc -o html5/index.html src/Main.c -Os -Wall /usr/local/lib/libraylib.a -I. -I/usr/local/include/raylib.h -L. -L/usr/local/lib/libraylib.a -s USE_GLFW=3 -DPLATFORM_WEB --preload-file assets/sfx/boing.wav --preload-file assets/bgm/01-Slipin-Sunday.ogg --preload-file assets/gfx/player.png --shell-file html5/shell.html
