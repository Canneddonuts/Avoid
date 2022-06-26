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
#define MAX_PLAYER_HP 5

struct Actor {
    float speed;
    int hp;
    int currentframe;
    Vector2 sprite_pos;
    Rectangle frameRec;
    Rectangle hitbox;
    Color color;
};

struct Item {
    Vector2 sprite_pos;
    Rectangle hitbox;
    bool active;
};

struct Actor player = { 0 };
struct Actor enemy = { 0 };
struct Actor fireworks[MAX_FIREWORKS] = { 0 };
struct Item heart = { 0 };
Sound fxbounce = { 0 };
bool pause;
bool mute;
bool player_in;

bool DebugMode;

#endif
