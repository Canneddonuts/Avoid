/*
-    Avoid ~ a game by Canneddonuts
-      Filename ~ Gameplay.h
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#ifndef GAMEPLAY_HEADER
#define GAMEPLAY_HEADER

struct Ball {
    Vector2 position;
    Vector2 speed;
    float radius;
    float growth;
    Color color;
    bool active;
};

struct Player {
    Texture2D sprite;
    float speed;
    int hp;
    int currentframe;
    Vector2 sprite_pos;
    Rectangle frameRec;
    Rectangle hitbox;
};

struct Item {
    Texture2D sprite;
    Vector2 sprite_pos;
    Rectangle hitbox;
    bool active;
};

struct Player player = { 0 };
struct Ball ball = { 0 };
struct Item heart = { 0 };
int pauseTimer;
Sound fxbounce = { 0 };
bool pause;
bool mute;
bool DebugMode;

#endif
