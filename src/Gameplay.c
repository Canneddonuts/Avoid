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
#include "Textures.h"

int score = 0, bestscore = 0;

void InitGameplayScreen(void)
{
  fxbounce = LoadSound("assets/sfx/boing.wav");

  SetMasterVolume(0.2);

  player.sprite = LoadTexture("assets/gfx/player.png");
  player.currentframe = 0;
  player.speed = 300.0f;
  player.hp = 30;
  player.frameRec = (Rectangle) {
    player.hitbox.x,
    player.hitbox.y,
   (float) player.sprite.width/3,
   (float) player.sprite.height
  };
  player.hitbox = (Rectangle) {
    GetScreenWidth()/2.0f - 30,
    GetScreenHeight()/2.0f - 30,
    (float) player.sprite.width/3,
    (float) player.sprite.height
  };

  heart.sprite = LoadTexture("assets/gfx/health.png");
  heart.hitbox = (Rectangle) {
    GetRandomValue(0, GetScreenWidth() - heart.sprite.width),
    GetRandomValue(0, GetScreenHeight() - heart.sprite.height),
    (float) heart.sprite.width,
    (float) heart.sprite.height
  };
  heart.active = true;

  ball.position = (Vector2){ 50, 50 };
  ball.speed = (Vector2){ 400.0f,  300.0f };
  ball.radius = 20;
  ball.growth = 2;
  ball.color = MAROON;
  ball.active = true;

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
   player.hp = 30;
   player.hitbox = (Rectangle) {
     GetScreenWidth()/2.0f - 30,
     GetScreenHeight()/2.0f - 30,
     (float) player.sprite.width/3,
     (float) player.sprite.height
   };

   heart.hitbox = (Rectangle) {
     GetRandomValue(0, GetScreenWidth() - heart.sprite.width),
     GetRandomValue(0, GetScreenHeight() - heart.sprite.height),
     (float) heart.sprite.width,
     (float) heart.sprite.height
   };
   heart.active = true;

   ball.position = (Vector2){ 50, 50 };
   ball.radius = 20;
   ball.active = true;

   DebugMode = 0;

   pauseTimer = 0;
   score = 0;
}

void UpdateGameplayScreen(void)
{
   if (IsKeyPressed(KEY_M)) mute = !mute;

   if (INPUT_OPTION_PRESSED) pause = !pause;

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


         player.sprite_pos = (Vector2){ player.hitbox.x, player.hitbox.y };
         player.frameRec.x = (float)player.currentframe*(float)player.sprite.width/3;

         heart.sprite_pos = (Vector2){ heart.hitbox.x, heart.hitbox.y };

         if (score % 1000 == 0) heart.active = true;

         // Player to da wallz collies
         if ((player.hitbox.x + player.hitbox.width) >= GetScreenWidth()) player.hitbox.x = GetScreenWidth() - player.hitbox.width;
         else if (player.hitbox.x <= 0) player.hitbox.x = 0;

         if ((player.hitbox.y + player.hitbox.height) >= GetScreenHeight()) player.hitbox.y = GetScreenHeight() - player.hitbox.height;
         else if (player.hitbox.y <= 0) player.hitbox.y = 0;

         if (IsKeyPressed(KEY_B)) ball.active = !ball.active;

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
                 player.hp = 30;
                 heart.hitbox.x = GetRandomValue(0, GetScreenWidth() - heart.sprite.width);
                 heart.hitbox.y = GetRandomValue(0, GetScreenHeight() - heart.sprite.height);
                 heart.active = false;
             }
         }

          if (ball.active) {
            score++;

            if (score >= bestscore)  bestscore = score;

            // movement of the ball
            ball.position.x += GetFrameTime() * ball.speed.x;
            ball.position.y += GetFrameTime() * ball.speed.y;

            // Ballz to da wallz collies
            if ((ball.position.x >= (GetScreenWidth() - ball.radius)) || (ball.position.x <= ball.radius)) {
              ball.speed.x *= -1.0f;
              if (!mute) PlaySoundMulti(fxbounce);
            }

            if ((ball.position.y >= (GetScreenHeight() - ball.radius)) || (ball.position.y <= ball.radius)) {
              ball.speed.y *= -1.0f;
              if (!mute) PlaySoundMulti(fxbounce);
            }

            if (CheckCollisionCircleRec(ball.position, ball.radius, player.hitbox)) {
              player.hp -= GetFrameTime() * 3.0f;
              player.currentframe = 1;
            } else player.currentframe = 0;

            if (ball.radius <= 100)  ball.radius += GetFrameTime() * ball.growth;
          }

       }
       else pauseTimer++;
}

void DrawGameplayScreen(void)
{
  DrawTexture(background, 0, 0, RAYWHITE);
  DrawFPS(10, 430);
  DrawText(TextFormat("HP: %i", player.hp), 10, 10, 20, RED);
  DrawText(TextFormat("SCORE: %i", score), 10, 30, 20, BLUE);
  if (DebugMode) {
    DrawText(TextFormat("BALL SIZE: %f", ball.radius), 10, 50, 20, GREEN);
    DrawText(TextFormat("BALL POS X: %f, BALL POS Y: %f", ball.position.x, ball.position.y), 10, 70, 20, GREEN);
    DrawText(TextFormat("BALL SPEED X: %f, BALL SPEED Y: %f", ball.speed.x, ball.speed.y), 10, 90, 20, GREEN);
    DrawRectangleRec(player.hitbox, BLUE);
    DrawRectangleRec(heart.hitbox, GREEN);
  }
  if (ball.active) DrawCircleV(ball.position, (float)ball.radius, ball.color);
  if (heart.active) DrawTexture(heart.sprite, heart.sprite_pos.x, heart.sprite_pos.y, RAYWHITE);
  DrawTextureRec(player.sprite, player.frameRec, player.sprite_pos, RAYWHITE);
  if (pause && ((pauseTimer/30)%2)) DrawText("PAUSED", 330, 190, 30, PURPLE);
}

void UnloadGameplayScreen()
{
  UnloadSound(fxbounce);
  UnloadTexture(player.sprite);
  UnloadTexture(heart.sprite);
}

void gameReset(void)
{
   ResetGameplayScreen();
   InitGameoverScreen();
}
