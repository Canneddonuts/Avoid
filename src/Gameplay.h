/*
-    Avoid ~ a game by Canneddonuts
-      Filename ~ Gameplay.h
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#ifndef GAMEPLAY_HEADER
#define GAMEPLAY_HEADER

#define MAX_FIREWORKS 10
#define MAX_SHOOTS 5

struct Actor {
    float speed;
    int hp;
    int currentframe;
    float iframetimer;
    Vector2 sprite_pos;
    Rectangle frameRec;
    Rectangle hitbox;
    Color color;
    Sound fxhit;
    bool in;
};

struct Item {
    Vector2 sprite_pos;
    Rectangle hitbox;
    bool active;
    Color color;
    int power;
};

struct Attack {
    Vector2 sprite_pos;
    Rectangle hitbox;
    Vector2 speed;
    int active;
    Color color;
};

struct Actor player = { 0 };
struct Actor enemy = { 0 };
struct Attack fireworks[MAX_FIREWORKS] = { 0 };
struct Attack shoot[MAX_SHOOTS] = { 0 };
struct Item feather = { 0 };
Sound fxfeather = { 0 };
bool pause;
bool DebugMode;
int ammo = 0;
int fireworkAmount = 0;
int GI_callcount = 0;

#endif
