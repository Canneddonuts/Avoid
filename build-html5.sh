#!/bin/sh
emcc -o game.html Main.c -Os -Wall /usr/local/lib/libraylib.a -I. -I/usr/local/include/raylib.h -L. -L/usr/local/lib/libraylib.a -s USE_GLFW=3 -s ASYNCIFY -DPLATFORM_WEB

