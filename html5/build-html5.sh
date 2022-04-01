#!/bin/sh
emcc -o index.html game.c -Os -Wall /usr/local/lib/libraylib.a -I. -I/usr/local/include/raylib.h -L. -L/usr/local/lib/libraylib.a -s USE_GLFW=3 -DPLATFORM_WEB --preload-file rec/boing.mp3 --preload-file rec/01-Slipin-Sunday.ogg #--shell-file minshell.html
