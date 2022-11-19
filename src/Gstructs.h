/*
-    Avoid ~ a game by Canneddonuts
-      Filename ~ Gstructs.h
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#ifndef GAMESTRUCTS_HEADER
#define GAMESTRUCTS_HEADER

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
    int hp;
    Color color;
};

void DamageActor(struct Actor *actor);
void UpdateiFrameTimer(struct Actor *actor);
bool CheckAttackActivity(struct Attack attack[], int val, int max);

#endif
