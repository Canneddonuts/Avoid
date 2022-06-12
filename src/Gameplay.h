/*
-    Avoid ~ a game by Canneddonuts
-      Filename ~ Gameplay.h
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#ifndef GAMEPLAY_HEADER
#define GAMEPLAY_HEADER

typedef struct Ball {
    Vector2 position;
    Vector2 speed;
    float radius;
    float growth;
    Color color;
    bool active;
} Ball;

typedef struct Player {
    Texture2D sprite;
    float speed;
    int hp;
    int currentframe;
    Vector2 sprite_pos;
    Rectangle frameRec;
    Rectangle hitbox;
} Player;

typedef struct Item {
    Texture2D sprite;
    Vector2 sprite_pos;
    Rectangle hitbox;
    bool active;
} Item;

int pauseTimer;
Sound fxbounce = { 0 };
Player player = { 0 };
Ball ball = { 0 };
Item heart = { 0 };
bool pause;
bool mute;
bool DebugMode;

#endif
