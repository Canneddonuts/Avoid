/*
-    Avoid ~ a game by Canneddonuts
-      Filename ~ Gameplay.c
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#include "../include/raylib.h"

#include "Screens.h"
#include "Controls.h"
#include "Gameplay.h"
#include "Score.h"
#include "Timers.h"
#include "Textures.h"

int score = 0, bestscore = 0;

void InitGameplayScreen(void)
{
  fxbounce = LoadSound("assets/sfx/boing.wav");

  SetMasterVolume(0.2);

  player_sprite = LoadTexture("assets/gfx/player.png");
  player.currentframe = 0;
  player.speed = 300.0f;
  player.hp = MAX_PLAYER_HP;
  player.frameRec = (Rectangle) {
    player.hitbox.x,
    player.hitbox.y,
   (float) player_sprite.width/3,
   (float) player_sprite.height
  };
  player.hitbox = (Rectangle) {
    0,
    50,
    (float) player_sprite.width/3,
    (float) player_sprite.height/2 + 5
  };
  player_iframeTimer = 0;
  player_in = false;
  player.color = RAYWHITE;
  player_flashtimer = 0;

  enemy_sprite = LoadTexture("assets/gfx/enemy.png");
  enemy.currentframe = 0;
  enemy.speed = 100.0f;
  enemy.hp = 30;
  enemy.hitbox = (Rectangle) {
    690,
    10,
    (float) enemy_sprite.width,
    (float) enemy_sprite.height
  };
  enemy.color = RAYWHITE;


  heart_sprite = LoadTexture("assets/gfx/health.png");
  heart.hitbox = (Rectangle) {
    GetRandomValue(0, GetScreenWidth() - heart_sprite.width),
    GetRandomValue(0, GetScreenHeight() - heart_sprite.height),
    (float) heart_sprite.width,
    (float) heart_sprite.height
  };
  heart.active = true;

  firework_sprite = LoadTexture("assets/gfx/firework.png");
  for (int i = 0; i < MAX_FIREWORKS; i++) {
    fireworks[i].currentframe = 0;
    fireworks[i].speed = 300.0f;
    fireworks[i].hp = 0;
    fireworks[i].hitbox = (Rectangle) {
      630,
      GetRandomValue(0, GetScreenHeight()),
      (float) firework_sprite.width/2 + 10,
      (float) firework_sprite.height
    };
    fireworks[i].color = RAYWHITE;
  }

  pause = 0;
  mute = true;
  DebugMode = 0;
  pauseTimer = 0;
}

void ResetGameplayScreen(void)
{
  // code to reset all variables without reloading assets
   player.currentframe = 0;
   player.speed = 300.0f;
   player.hp = MAX_PLAYER_HP;
   player.hitbox = (Rectangle) {
     0,
     50,
     (float) player_sprite.width/3,
     (float) player_sprite.height/2 + 5
   };
   player_iframeTimer = 0;
   player_in = false;
   player.color = RAYWHITE;
   player_flashtimer = 0;

   enemy.currentframe = 0;
   enemy.speed = 100.0f;
   enemy.hp = 30;
   enemy.hitbox = (Rectangle) {
     690,
     10,
     (float) enemy_sprite.width,
     (float) enemy_sprite.height
   };

   heart.hitbox = (Rectangle) {
     GetRandomValue(0, GetScreenWidth() - heart_sprite.width),
     GetRandomValue(0, GetScreenHeight() - heart_sprite.height),
     (float) heart_sprite.width,
     (float) heart_sprite.height
   };
   heart.active = true;

   for (int i = 0; i < MAX_FIREWORKS; i++) {
     fireworks[i].currentframe = 0;
     fireworks[i].speed = 300.0f;
     fireworks[i].hp = 0;
     fireworks[i].hitbox = (Rectangle) {
       630,
       GetRandomValue(0, GetScreenHeight()),
       (float) firework_sprite.width/2 + 10,
       (float) firework_sprite.height
     };
   }

   DebugMode = 0;

   pauseTimer = 0;
   score = 0;
}

void DamagePlayer(void)
{
  if (!player_in) {
    player.hp--;
    player_in = true;
  }

  player.currentframe = 1;
}

void UpdateiFrameTimer(void)
{
  if (player_in) {
    player_iframeTimer++;
    if (player_flashtimer % 2 == 0) player.color = BLANK;
    else player.color = RAYWHITE;
    if (player_iframeTimer >= 60) {
      player_in = false;
      player_iframeTimer = 0;
    }
  } else player.color = RAYWHITE;
}

void UpdateGameplayScreen(void)
{
   if (IsKeyPressed(KEY_M)) mute = !mute;

   if (INPUT_OPTION_PRESSED) pause = !pause;

   if (!pause) {

       if (INPUT_LEFT_DOWN) player.hitbox.x -= GetFrameTime() * player.speed;
       if (INPUT_RIGHT_DOWN) player.hitbox.x += GetFrameTime() * player.speed;
       if (INPUT_UP_DOWN) player.hitbox.y -= GetFrameTime() * player.speed;
       if (INPUT_DOWN_DOWN) player.hitbox.y += GetFrameTime() * player.speed;
       if (INPUT_DASH_DOWN) {
         player.speed = 600.0f;
         if (player.currentframe != 1) player.currentframe = 2;
       } else player.speed = 300.0f;

         player.sprite_pos = (Vector2){ player.hitbox.x, player.hitbox.y };
         player.frameRec.x = (float)player.currentframe*(float)player_sprite.width/3;

         heart.sprite_pos = (Vector2){ heart.hitbox.x, heart.hitbox.y };
         enemy.sprite_pos = (Vector2){ enemy.hitbox.x, enemy.hitbox.y };

         player_flashtimer++;

         for (int i = 0; i < MAX_FIREWORKS; i++) {
           fireworks[i].sprite_pos = (Vector2){ fireworks[i].hitbox.x, fireworks[i].hitbox.y };
         }

         if (score % 1000 == 0) heart.active = true;

         // Player to da wallz collies
         if ((player.hitbox.x + player.hitbox.width) >= GetScreenWidth()) player.hitbox.x = GetScreenWidth() - player.hitbox.width;
         else if (player.hitbox.x <= 0) player.hitbox.x = 0;

         if ((player.hitbox.y + player.hitbox.height) >= GetScreenHeight()) player.hitbox.y = GetScreenHeight() - player.hitbox.height;
         else if (player.hitbox.y <= 0) player.hitbox.y = 0;

         UpdateiFrameTimer();

         if (IsKeyPressed(KEY_D)) DebugMode = !DebugMode;

         if (IsKeyPressed(KEY_R)) {
           gameReset();
           currentScreen = TITLE;
         }

         if (player.hp <= 0) {
           gameReset();
           currentScreen = GAMEOVER;
         }

         if (heart.active) {
             if (CheckCollisionRecs(player.hitbox,  heart.hitbox)) {
                 player.hp = MAX_PLAYER_HP;
                 heart.hitbox.x = GetRandomValue(0, GetScreenWidth() - heart_sprite.width);
                 heart.hitbox.y = GetRandomValue(0, GetScreenHeight() - heart_sprite.height);
                 heart.active = false;
             }
         }

         if (enemy.hp != 0) {
           score++;
           if (score >= bestscore)  bestscore = score;

           enemy.hitbox.y += GetFrameTime() * enemy.speed;

           if (((enemy.hitbox.y + enemy.hitbox.height) >= GetScreenHeight()
           || (enemy.hitbox.y <= 0))) enemy.speed *= -1.0f;

           if (CheckCollisionRecs(player.hitbox, enemy.hitbox)) DamagePlayer();
           else player.currentframe = 0;
         }

         for (int i = 0; i < MAX_FIREWORKS; i++) {
           if (CheckCollisionRecs(player.hitbox, fireworks[i].hitbox)) DamagePlayer();

           switch (fireworks[i].hp) {
             case 0:
                 fireworks[i].hitbox.x = enemy.hitbox.x - 20;
                 fireworks[i].hitbox.y = enemy.hitbox.y - 20;

                 if (GetRandomValue(0, 50) == 50) fireworks[i].hp = 1;
                 break;
             case 1:
               fireworks[i].hitbox.x += GetFrameTime() * -fireworks[i].speed;
               if (((fireworks[i].hitbox.x + -firework_sprite.width) > GetScreenWidth()
               || (fireworks[i].hitbox.x <= -firework_sprite.width))) fireworks[i].hp = 0;
               break;
           }
         }

       }
       else pauseTimer++;
}

void DrawGameplayScreen(void)
{
  DrawTexture(background, 0, 0, RAYWHITE);
  DrawFPS(10, 430);
  if (DebugMode) {
    DrawRectangleRec(player.hitbox, BLUE);
    DrawRectangleRec(heart.hitbox, GREEN);
    DrawText(TextFormat("enemy.hitbox.y: %f", enemy.hitbox.y), 10, 200, 20, GREEN);
    DrawText(TextFormat("player.hitbox.y: %f", player.hitbox.y), 10, 230, 20, GREEN);
    DrawRectangleRec(enemy.hitbox, BLACK);
    DrawText(TextFormat("firework_sprite.width: %d", firework_sprite.width), 10, 270, 20, GREEN);
    for (int i = 0; i < MAX_FIREWORKS; i++) {
      DrawText(TextFormat("fireworks[i].hp: %d", fireworks[i].hp), 10, 250, 20, GREEN);
      DrawRectangleRec(fireworks[i].hitbox, BLACK);
    }
    DrawText(TextFormat("player_iframeTimer: %d", player_iframeTimer), 10, 300, 20, GREEN);
    DrawText(TextFormat("player_in: %d", player_in), 10, 320, 20, GREEN);
  }
  if (heart.active) DrawTexture(heart_sprite, heart.sprite_pos.x, heart.sprite_pos.y, RAYWHITE);
  DrawTexture(enemy_sprite, enemy.sprite_pos.x, enemy.sprite_pos.y, enemy.color);
  for (int i = 0; i < MAX_FIREWORKS; i++) {
    DrawTexture(firework_sprite, fireworks[i].sprite_pos.x, fireworks[i].sprite_pos.y, fireworks[i].color);
  }
  DrawTextureRec(player_sprite, player.frameRec, player.sprite_pos, player.color);
  DrawText(TextFormat("HP: %i", player.hp), 10, 10, 20, RED);
  DrawText(TextFormat("SCORE: %i", score), 10, 30, 20, BLUE);
  if (pause && ((pauseTimer/30)%2)) DrawText("PAUSED", 330, 190, 30, PURPLE);
}

void UnloadGameplayScreen()
{
  UnloadSound(fxbounce);
  UnloadTexture(player_sprite);
  UnloadTexture(heart_sprite);
  UnloadTexture(enemy_sprite);
  UnloadTexture(firework_sprite);
}

void gameReset(void)
{
   ResetGameplayScreen();
   InitGameoverScreen();
}
