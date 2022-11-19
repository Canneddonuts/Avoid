/*
-    Avoid ~ a game by Canneddonuts
-      Filename ~ Gameplay.c
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#include "../include/raylib.h"
#include <math.h>

#include "Screens.h"
#include "Controls.h"
#include "Options.h"
#include "Gstructs.h"
#include "Stats.h"
#include "Timers.h"
#include "Music.h"
#include "Gfx.h"

#define MAX_FIREWORKS 10
#define MAX_SHOOTS 5

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
int trigMov;

int score = 0, bestscore = 0, finishfromGameplayScreen = 0, redfeathers = 0, greenfeathers = 0;

Music Gameplaysong = { 0 };

void LoadGamplayScreen(void)
{
  player.fxhit = LoadSound("assets/sfx/hit.wav");
  enemy.fxhit = LoadSound("assets/sfx/boom.wav");
  player_sprite = LoadTexture("assets/gfx/player.png");
  enemy_sprite = LoadTexture("assets/gfx/enemy.png");
  fxfeather = LoadSound("assets/sfx/feather.wav");
  feather_sprite = LoadTexture("assets/gfx/feather.png");
  attack_sprite = LoadTexture("assets/gfx/attack.png");
  firework_sprite = LoadTexture("assets/gfx/firework.png");
  Gameplaysong = LoadMusicStream("assets/bgm/03-Boss.ogg");
}

void InitGameplayScreen(void)
{
  PlayMusicStream(Gameplaysong);

  finishfromGameplayScreen = 0;

  nextlevel = level + 1;

  globalTimer = 0;

  if (player.hp < 1) player.hp = 1;
  //if (ammo < 60) ammo = 60;

  player.currentframe = 0;
  player.speed = 300.0f;
  if (GI_callcount < 1) {
    player.frameRec = (Rectangle) {
      player.hitbox.x,
      player.hitbox.y,
     (float) player_sprite.width/3,
     (float) player_sprite.height
    };
  }
  player.hitbox = (Rectangle) {
    0,
    100,
    (float) player_sprite.width/3 - 20,
    (float) player_sprite.height - 20
  };
  player.iframetimer = 0;
  player.in = false;
  player.color = RAYWHITE;

  enemy.currentframe = 0;
  enemy.hp = 20;
  enemy.speed = 200.0f;
  if (GI_callcount < 1) {
    enemy.frameRec = (Rectangle) {
      enemy.hitbox.x,
      enemy.hitbox.y,
      (float) enemy_sprite.width/2,
      (float) enemy_sprite.height
    };
  }
  enemy.hitbox = (Rectangle) {
    690,
    20,
    (float) enemy_sprite.width/2,
    (float) enemy_sprite.height
  };
  enemy.color = RAYWHITE;
  enemy.in = false;
  enemy.iframetimer = 0;

  feather.hitbox = (Rectangle) {
    GetScreenWidth() - feather_sprite.width,
    GetRandomValue(0, GetScreenHeight() - feather_sprite.height),
    (float) feather_sprite.width,
    (float) feather_sprite.height
  };
  feather.active = false;
  feather.power = 0;

  for (int i = 0; i < MAX_FIREWORKS; i++) {
    fireworks[i].active = 1;
    fireworks[i].hp = 5;
    fireworks[i].hitbox = (Rectangle) {
      GetScreenWidth() + firework_sprite.width,
      0,
      (float) firework_sprite.width/2 + 10,
      (float) firework_sprite.height
    };
    fireworks[i].hitbox.y = GetRandomValue(0, GetScreenHeight() - firework_sprite.height);
    switch (level) {
      case LEVEL1: fireworks[i].speed.x = GetRandomValue(100, 300); break;
      case LEVEL2: fireworks[i].speed.x = GetRandomValue(600, 800); break;
      case LEVEL3: fireworks[i].speed.x = GetRandomValue(1200, 2400); break;
    } 
    fireworks[i].color = RAYWHITE;
  }

  for (int i = 0; i < MAX_SHOOTS; i++) {
    shoot[i].hitbox = (Rectangle) {
      player.hitbox.x,
      player.hitbox.y,
      (float) attack_sprite.width,
      (float) attack_sprite.height
    };
    shoot[i].speed.x = 5000.f;
    shoot[i].speed.y = 0;
    shoot[i].active = false;
    shoot[i].color = RED;
  }
  switch (level) {
    case LEVEL1: fireworkAmount = 100; break;
    case LEVEL2: fireworkAmount = 150; break;
    case LEVEL3: fireworkAmount = 200; break;
  }

  pause = 0;
  DebugMode = 0;
  pauseTimer = 0;


  GI_callcount++;
}

void ResetFeather(void)
{
  feather.power = 0;

  feather.hitbox.x = GetScreenWidth() + feather_sprite.width;
  feather.hitbox.y = GetRandomValue(0, GetScreenHeight() - feather_sprite.height);
  feather.active = false;
}

void UpdateGameplayScreen(void)
{
   if (INPUT_OPTION_PRESSED) pause = !pause;
   // code to end the game
   if (level > 2) { StopMusicStream(Gameplaysong); finishfromGameplayScreen = 3; }
   if (CheckAttackActivity(fireworks, 0, MAX_FIREWORKS) && level < 2) { StopMusicStream(Gameplaysong); levelunlocked[nextlevel] = true; finishfromGameplayScreen = 4; }

   if (!mute) UpdateMusicStream(Gameplaysong);

   if (!pause) {

       // Controls
       if (INPUT_LEFT_DOWN) player.hitbox.x -= GetFrameTime() * player.speed;
       if (INPUT_RIGHT_DOWN) player.hitbox.x += GetFrameTime() * player.speed;
       if (INPUT_UP_DOWN) player.hitbox.y -= GetFrameTime() * player.speed;
       if (INPUT_DOWN_DOWN) player.hitbox.y += GetFrameTime() * player.speed;
       if (INPUT_DASH_DOWN) {
         player.speed = 600.0f;
         if (player.currentframe != 1) player.currentframe = 2;
       } else player.speed = 300.0f;
       if (INPUT_FIRE_DOWN) {
        // if (ammo > 0) {
           for (int i = 0; i < MAX_SHOOTS; i++) {
             if (!shoot[i].active) {
               ammo++;
               shoot[i].hitbox.x = player.hitbox.x;
               shoot[i].hitbox.y = player.hitbox.y + player.hitbox.height/4;
               shoot[i].active = true;
               break;
             }
           }
       //  }
       }
         // Update sprite positions
         player.sprite_pos = (Vector2){ player.hitbox.x, player.hitbox.y };
         player.frameRec.x = (float)player.currentframe*(float)player_sprite.width/3;
         feather.sprite_pos = (Vector2){ feather.hitbox.x, feather.hitbox.y };
         enemy.sprite_pos = (Vector2){ enemy.hitbox.x, enemy.hitbox.y };
         enemy.frameRec.x = (float)enemy.currentframe*(float)enemy_sprite.width/2;

         for (int i = 0; i < MAX_FIREWORKS; i++) {
           fireworks[i].sprite_pos = (Vector2){ fireworks[i].hitbox.x, fireworks[i].hitbox.y };
         }

         for (int i = 0; i < MAX_SHOOTS; i++) {
           shoot[i].sprite_pos = (Vector2){ shoot[i].hitbox.x, shoot[i].hitbox.y };
         }

         // Player wall collision
         if ((player.hitbox.x + player.hitbox.width) >= GetScreenWidth()) player.hitbox.x = GetScreenWidth() - player.hitbox.width;
         else if (player.hitbox.x <= 0) player.hitbox.x = 0;

         if ((player.hitbox.y + player.hitbox.height) >= GetScreenHeight()) player.hitbox.y = GetScreenHeight() - player.hitbox.height;
         else if (player.hitbox.y <= 0) player.hitbox.y = 0;

         // Update Timers
         scoreTimer += 60 * GetFrameTime();
         score = (int)scoreTimer;
         globalTimer += 10 * GetFrameTime();
         // pass the address of each struct to the UpdateiFrameTimer function
         UpdateiFrameTimer(&player);
         UpdateiFrameTimer(&enemy);
         greenfeathers = player.hp;
         redfeathers = ammo;

         // Debug stuff
         if (IsKeyPressed(KEY_D)) DebugMode = !DebugMode;
         if (IsKeyPressed(KEY_NINE)) ammo = 99;
         if (IsKeyPressed(KEY_ZERO)) ammo = 0;
         if (IsKeyPressed(KEY_G)) finishfromGameplayScreen = 1;
         if (IsKeyPressed(KEY_R)) finishfromGameplayScreen = 2;
         if (IsKeyPressed(KEY_W)) finishfromGameplayScreen = 3;
         if (IsKeyPressed(KEY_EQUAL)) level++;
         if (IsKeyPressed(KEY_MINUS)) level--;

         // call gameover when killed
         if (player.hp < 1) { StopMusicStream(Gameplaysong); finishfromGameplayScreen = 1; }

         // Red feather logic
         for (int i = 0; i < MAX_SHOOTS; i++) {
           if (shoot[i].active) {
             shoot[i].hitbox.x += shoot[i].speed.x * GetFrameTime();
           }
           if (shoot[i].hitbox.x + shoot[i].hitbox.width >= GetScreenWidth() + attack_sprite.width) shoot[i].active = false;
         }

          // Feather spawn logic
          if (level == LEVEL3) { if ((int) globalTimer % 10 == 0) feather.active = true; }
          else { if ((int) globalTimer % 30 == 0) feather.active = true; }
          switch (feather.power) {
             case 0:  feather.color = GREEN; break;
             case 1:  feather.color = RED; break;
           }
           if (feather.active) {
             if (((feather.hitbox.x + -feather_sprite.width) > GetScreenWidth()
             || (feather.hitbox.x <= -feather_sprite.width)))  ResetFeather();
             if (CheckCollisionRecs(player.hitbox,  feather.hitbox)) {
                 switch (feather.power) {
                   case 0: player.hp++;  break;
                   case 1: ammo += 60; break;
                 }
                 if (!mute) PlaySoundMulti(fxfeather);
                 ResetFeather();
             }
             feather.hitbox.x -= 300.0f * GetFrameTime();
           }

         // Enemy logic
         if (level == 2) {
           if ((int)globalTimer % 40 == 0) enemy.hitbox.y = GetRandomValue(0, GetScreenHeight() - enemy_sprite.height);

           if (CheckCollisionRecs(player.hitbox, enemy.hitbox)) DamageActor(&player);

           for (int i = 0; i < MAX_SHOOTS; i++) {
             if (CheckCollisionRecs(shoot[i].hitbox, enemy.hitbox) && shoot[i].active) {
               DamageActor(&enemy);
               scoreTimer += 300;
               enemy.hitbox.y = GetRandomValue(0, GetScreenHeight());
               shoot[i].active = false;
             }
              if (((shoot[i].hitbox.x + -attack_sprite.width) > GetScreenWidth()
              || (shoot[i].hitbox.x <= -attack_sprite.width)))  shoot[i].active = 0;
           }

           if (enemy.hp < 1) { level++; enemy.hp = 5; }
         }

         // Firework logic
          for (int i = 0; i < MAX_FIREWORKS; i++) {
             if (CheckCollisionRecs(player.hitbox, fireworks[i].hitbox)) {
               DamageActor(&player);
               fireworks[i].active = 0;
             }
             for (int j = 0; j < MAX_SHOOTS; j++) {
               if (CheckCollisionRecs(shoot[j].hitbox, fireworks[i].hitbox) && shoot[j].active) {
                 if (!mute) PlaySoundMulti(enemy.fxhit);
                 fireworks[i].color = BLACK;
                 shoot[j].active = 0;
                 fireworks[i].hp--;
                 scoreTimer += 300;
               } else fireworks[i].color = RAYWHITE;
             }
             switch (fireworks[i].active) {
                case 0:
                    fireworks[i].hitbox.x = GetScreenWidth() + firework_sprite.width;

                    fireworks[i].hp = 5;

                    if (fireworkAmount > 0) { /*fireworkAmount--;*/ fireworks[i].active = 1; }
                    fireworks[i].hitbox.y = GetRandomValue(0, GetScreenHeight() - firework_sprite.height);
                   /* switch (level) {
                      case LEVEL1: fireworks[i].speed.x = GetFrameTime() break;
                      case LEVEL2: fireworks[i].speed.x = GetRandomValue(400, 600); break;
                      case LEVEL3: fireworks[i].speed.x = GetRandomValue(800, 1000); break;
                    } */
                    break;
                case 1:
                  if (fireworks[i].hp < 1) { fireworkAmount--; fireworks[i].active = 0; }
                  trigMov = sin(2*PI/20*fireworks[i].hitbox.x) * 200;
                  fireworks[i].hitbox.x -= fireworks[i].speed.x * GetFrameTime();
                  fireworks[i].hitbox.y += trigMov*GetFrameTime();
                  // Firework wall collision
                  if (((fireworks[i].hitbox.x + -firework_sprite.width) > GetScreenWidth()
                  || (fireworks[i].hitbox.x <= -firework_sprite.width)))  fireworks[i].active = 0;
                  break;
              }
         }
       } else pauseTimer += 60 * GetFrameTime();
}

void DrawGameplayScreen(void)
{
  switch (level) {
    case LEVEL1: DrawTexture(background, 0, 0, RAYWHITE); break;
    case LEVEL2: DrawTexture(background, 0, 0, ORANGE); break;
    case LEVEL3: DrawTexture(background, 0, 0, RED); break;
  }
  DrawFPS(10, 430);
  if (DebugMode) {
    DrawRectangleLines(player.hitbox.x, player.hitbox.y, player.hitbox.width, player.hitbox.height, BLUE);
    DrawRectangleLines(feather.hitbox.x, feather.hitbox.y, feather.hitbox.width, feather.hitbox.height, WHITE);
    DrawRectangleLines(enemy.hitbox.x, enemy.hitbox.y, enemy.hitbox.width, enemy.hitbox.height, BLACK);
    for (int i = 0; i < MAX_FIREWORKS; i++) {
      DrawRectangleLines(fireworks[i].hitbox.x, fireworks[i].hitbox.y, fireworks[i].hitbox.width, fireworks[i].hitbox.height, BLACK);
    }
    for (int i = 0; i < MAX_SHOOTS; i++) {
      DrawRectangleLines(shoot[i].hitbox.x, shoot[i].hitbox.y, shoot[i].hitbox.width, shoot[i].hitbox.height, GREEN);
    }
    DrawText(TextFormat("enemy.hitbox.y: %f", enemy.hitbox.y), 10, 200, 20, GREEN);
    DrawText(TextFormat("enemy.speed: %f", enemy.speed), 10, 220, 20, GREEN);
    DrawText(TextFormat("globalTimer: %f", globalTimer), 10, 240, 20, GREEN);
    DrawText(TextFormat("firework_sprite.width: %d", firework_sprite.width), 10, 260, 20, GREEN);
    DrawText(TextFormat("player.iframetimer: %f", player.iframetimer), 10, 280, 20, GREEN);
    DrawText(TextFormat("player.in: %d", player.in), 10, 300, 20, GREEN);
    DrawText(TextFormat("feather.active: %d", feather.active), 10, 320, 20, GREEN);
    DrawText(TextFormat("GetTime(): %f", GetTime()), 10, 340, 20, GREEN);
    DrawText(TextFormat("fireworkAmount: %d", fireworkAmount), 10, 360, 20, GREEN);
  }
  if (feather.active) DrawTexture(feather_sprite, feather.sprite_pos.x, feather.sprite_pos.y, feather.color);
  for (int i = 0; i < MAX_FIREWORKS; i++) {
    DrawTexture(firework_sprite, fireworks[i].sprite_pos.x, fireworks[i].sprite_pos.y, fireworks[i].color);
  }
  for (int i = 0; i < MAX_SHOOTS; i++) {
    if (shoot[i].active) DrawTexture(attack_sprite, shoot[i].sprite_pos.x, shoot[i].sprite_pos.y, shoot[i].color);
  }
  if (level == 2) DrawTextureRec(enemy_sprite, enemy.frameRec, enemy.sprite_pos, enemy.color);
  DrawTextureRec(player_sprite, player.frameRec, player.sprite_pos, player.color);
  DrawTexture(feather_sprite, 0, 0, GREEN);
  DrawText(TextFormat("= %i", player.hp), 30, 30, 30, GREEN);
  DrawTexture(feather_sprite, 80, 0, RED);
  DrawText(TextFormat("= %i", ammo), 110, 30, 30, RED);
  if (level == 2)  DrawText(TextFormat("ENEMY HP: %i", enemy.hp), GetScreenWidth() - 380, 0, 20, RED);
  DrawText(TextFormat("FIREWORKS LEFT: %i", fireworkAmount), GetScreenWidth() - 240, 0, 20, GREEN);
  if (score >= 10000) DrawText(TextFormat("SCORE: %i", score), 10, 65, 30, (Color){ 222, 181, 0, 255 });
  else DrawText(TextFormat("SCORE: %i", score), 10, 65, 30, BLUE);
  if (pause && (((int)pauseTimer/30)%2)) DrawTextEx(ZadoBold, "PAUSED", (Vector2){ 280, 160 }, 60, 2, WHITE);
}

void UnloadGameplayScreen(void)
{
  UnloadSound(player.fxhit);
  UnloadSound(enemy.fxhit);
  UnloadSound(fxfeather);
  UnloadTexture(player_sprite);
  UnloadTexture(feather_sprite);
  UnloadTexture(enemy_sprite);
  UnloadTexture(firework_sprite);
  UnloadTexture(attack_sprite);
  UnloadMusicStream(Gameplaysong);
}

int FinishGameplayScreen(void)
{
  return finishfromGameplayScreen;
}
