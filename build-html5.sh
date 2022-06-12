#!/bin/sh
emcc -o html5/index.html src/Main.c src/Title.c src/Credits.c src/Gameover.c src/Gameplay.c -Os -Wall /usr/local/lib/libraylib.a -I. -I/usr/local/include/raylib.h -L. -L/usr/local/lib/libraylib.a -s USE_GLFW=3 -DPLATFORM_WEB --preload-file ./assets --shell-file html5/shell.html
