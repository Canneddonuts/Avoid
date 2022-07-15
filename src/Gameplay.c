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

void LoadGamplayScreen(void)
{
  fxhit = LoadSound("assets/sfx/hit.wav");
  player_sprite = LoadTexture("assets/gfx/player.png");
  enemy_sprite = LoadTexture("assets/gfx/enemy.png");
  fxfeather = LoadSound("assets/sfx/feather.wav");
  feather_sprite = LoadTexture("assets/gfx/feather.png");
  attack_sprite = LoadTexture("assets/gfx/attack.png");
  firework_sprite = LoadTexture("assets/gfx/firework.png");
  fxboom = LoadSound("assets/sfx/boom.wav");
}

void InitGameplayScreen(void)
{
  SetMasterVolume(0.5);

  player.currentframe = 0;
  player.speed = 300.0f;
  player.hp = PLAYER_HP;
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
    (float) player_sprite.width/3,
    (float) player_sprite.height/2 +5
  };
  player_iframeTimer = 0;
  player_in = false;
  player.color = RAYWHITE;
  player_flashtimer = 0;

  enemy.currentframe = 0;
  enemy.speed = 2.0f;
  enemy.hp = 30;
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
    10,
    (float) enemy_sprite.width/2,
    (float) enemy_sprite.height
  };
  enemy.color = RAYWHITE;
  enemy_hurt = false;


  feather.hitbox = (Rectangle) {
    GetRandomValue(0, GetScreenWidth() - feather_sprite.width),
    GetRandomValue(0, GetScreenHeight() - feather_sprite.height),
    (float) feather_sprite.width,
    (float) feather_sprite.height
  };
  feather.active = true;
  feather.power = 0;


  for (int i = 0; i < MAX_FIREWORKS; i++) {
    fireworks[i].speed.x = 300.0f;
    fireworks[i].active = 0;
    fireworks[i].hitbox = (Rectangle) {
      630,
      GetRandomValue(0, GetScreenHeight()),
      (float) firework_sprite.width/2 + 10,
      (float) firework_sprite.height
    };
    fireworks[i].color = RAYWHITE;
  }

  for (int i = 0; i < MAX_SHOOTS; i++) {
    shoot[i].hitbox = (Rectangle) {
      player.hitbox.x,
      player.hitbox.y,
      (float) attack_sprite.width,
      (float) attack_sprite.height
    };
    shoot[i].speed.x = 7;
    shoot[i].speed.y = 0;
    shoot[i].active = false;
    shoot[i].color = RED;
  }
  ammo = 0;

  pause = 0;
  DebugMode = 0;
  pauseTimer = 0;
  score = 0;

  GI_callcount++;
}

void DamagePlayer(void)
{
  if (!player_in) {
    player.hp--;
    if (!mute) PlaySoundMulti(fxhit);
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

  if (enemy_hurt) {
    enemy_frametimer++;
    enemy.currentframe = 1;
    if (enemy_frametimer >= 60) {
      enemy_hurt = false;
      enemy_frametimer = 0;
    }
  } else enemy.currentframe = 0;
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
       if (INPUT_FIRE_PRESSED) {
         if (ammo > 0) {
           for (int i = 0; i < MAX_SHOOTS; i++) {
             if (!shoot[i].active) {
               ammo--;
               shoot[i].hitbox.x = player.hitbox.x;
               shoot[i].hitbox.y = player.hitbox.y + player.hitbox.height/4;
               shoot[i].active = true;
               break;
             }
           }
         }
       }

         player.sprite_pos = (Vector2){ player.hitbox.x, player.hitbox.y };
         player.frameRec.x = (float)player.currentframe*(float)player_sprite.width/3;

         feather.sprite_pos = (Vector2){ feather.hitbox.x, feather.hitbox.y };
         enemy.sprite_pos = (Vector2){ enemy.hitbox.x, enemy.hitbox.y };
         enemy.frameRec.x = (float)enemy.currentframe*(float)enemy_sprite.width/2;


         player_flashtimer++;

         for (int i = 0; i < MAX_FIREWORKS; i++) {
           fireworks[i].sprite_pos = (Vector2){ fireworks[i].hitbox.x, fireworks[i].hitbox.y };
         }

         for (int i = 0; i < MAX_SHOOTS; i++) {
           shoot[i].sprite_pos = (Vector2){ shoot[i].hitbox.x, shoot[i].hitbox.y };
         }


         if (score % 500 == 0) feather.active = true;

         // Player to da wallz collies
         if ((player.hitbox.x + player.hitbox.width) >= GetScreenWidth()) player.hitbox.x = GetScreenWidth() - player.hitbox.width;
         else if (player.hitbox.x <= 0) player.hitbox.x = 0;

         if ((player.hitbox.y + player.hitbox.height) >= GetScreenHeight()) player.hitbox.y = GetScreenHeight() - player.hitbox.height;
         else if (player.hitbox.y <= 0) player.hitbox.y = 0;

         UpdateiFrameTimer();

         if (IsKeyPressed(KEY_D)) DebugMode = !DebugMode;
         if (IsKeyPressed(KEY_NINE)) ammo = 99;
         if (IsKeyPressed(KEY_ZERO)) ammo = 0;



         if (IsKeyPressed(KEY_R)) {
           gameReset();
           currentScreen = TITLE;
         }

         if (player.hp <= 0) {
           gameReset();
           currentScreen = GAMEOVER;
         }

         for (int i = 0; i < MAX_SHOOTS; i++) {
           if (shoot[i].active) {
             shoot[i].hitbox.x += shoot[i].speed.x;
           }

           if (CheckCollisionRecs(shoot[i].hitbox, enemy.hitbox) && shoot[i].active) {
            // enemy.hp--;
             enemy_hurt = true;
             score += 300;
             if (!mute) PlaySoundMulti(fxboom);
             shoot[i].active = false;
           }

           if (shoot[i].hitbox.x + shoot[i].hitbox.width >= GetScreenWidth() + attack_sprite.width) {
             shoot[i].active = false;
           }
         }

         if (feather.active) {
           switch (feather.power) {
             case 0:  feather.color = GREEN; break;
             case 1:  feather.color = RED; break;
           }
             if (CheckCollisionRecs(player.hitbox,  feather.hitbox)) {
                 switch (feather.power) {
                   case 0: player.hp++; feather.power = 1; break;
                   case 1: ammo++; feather.power = 0; break;
                 }
                 if (!mute) PlaySoundMulti(fxfeather);
                 feather.hitbox.x = GetRandomValue(0, GetScreenWidth() - feather_sprite.width);
                 feather.hitbox.y = GetRandomValue(0, GetScreenHeight() - feather_sprite.height);
                 feather.active = false;
             }
         }

         if (enemy.hp != 0) {
           score++;
           if (score >= bestscore)  bestscore = score;

           enemy.hitbox.y += enemy.speed;

           if (((enemy.hitbox.y + enemy.hitbox.height) >= GetScreenHeight()
           || (enemy.hitbox.y <= 0))) enemy.speed *= -1.0f;

           if (CheckCollisionRecs(player.hitbox, enemy.hitbox)) DamagePlayer();
           else player.currentframe = 0;
         }

         for (int i = 0; i < MAX_FIREWORKS; i++) {
           if (CheckCollisionRecs(player.hitbox, fireworks[i].hitbox)) {
             DamagePlayer();
             fireworks[i].active = 0;
           }

           switch (fireworks[i].active) {
             case 0:
                 fireworks[i].hitbox.x = enemy.hitbox.x - 20;
                 fireworks[i].hitbox.y = enemy.hitbox.y - 20;

                 if (GetRandomValue(0, 50) == 50) {
                   fireworks[i].active = 1;
                   fireworks[i].hitbox.y += enemy.hitbox.height/2;
                 }
                 break;
             case 1:
               fireworks[i].hitbox.x += GetFrameTime() * -fireworks[i].speed.x;
               if (((fireworks[i].hitbox.x + -firework_sprite.width) > GetScreenWidth()
               || (fireworks[i].hitbox.x <= -firework_sprite.width))) fireworks[i].active = 0;
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
    DrawRectangleRec(feather.hitbox, WHITE);
    DrawText(TextFormat("enemy.hitbox.y: %f", enemy.hitbox.y), 10, 200, 20, GREEN);
    DrawText(TextFormat("player.hitbox.y: %f", player.hitbox.y), 10, 230, 20, GREEN);
    DrawRectangleRec(enemy.hitbox, BLACK);
    DrawText(TextFormat("firework_sprite.width: %d", firework_sprite.width), 10, 270, 20, GREEN);
    for (int i = 0; i < MAX_FIREWORKS; i++) {
      DrawRectangleRec(fireworks[i].hitbox, BLACK);
    }
    for (int i = 0; i < MAX_SHOOTS; i++) {
      DrawRectangleRec(shoot[i].hitbox, GREEN);
      DrawText(TextFormat("ammo: %d", ammo), 10, 340, 20, GREEN);
    }
    DrawText(TextFormat("player_iframeTimer: %d", player_iframeTimer), 10, 300, 20, GREEN);
    DrawText(TextFormat("player_in: %d", player_in), 10, 320, 20, GREEN);
  }
  if (feather.active) DrawTexture(feather_sprite, feather.sprite_pos.x, feather.sprite_pos.y, feather.color);
  DrawTextureRec(enemy_sprite, enemy.frameRec, enemy.sprite_pos, enemy.color);
  for (int i = 0; i < MAX_FIREWORKS; i++) {
    DrawTexture(firework_sprite, fireworks[i].sprite_pos.x, fireworks[i].sprite_pos.y, fireworks[i].color);
  }
  for (int i = 0; i < MAX_SHOOTS; i++) {
    if (shoot[i].active) DrawTexture(attack_sprite, shoot[i].sprite_pos.x, shoot[i].sprite_pos.y, shoot[i].color);
  }
  DrawTextureRec(player_sprite, player.frameRec, player.sprite_pos, player.color);
  DrawTexture(feather_sprite, 0, 0, GREEN);
  DrawText(TextFormat("= %i", player.hp), 30, 30, 20, GREEN);
  DrawTexture(feather_sprite, 70, 0, RED);
  DrawText(TextFormat("= %i", ammo), 100, 30, 20, RED);
//  DrawText(TextFormat("ENEMY HP: %i", enemy.hp), GetScreenWidth() - 150, 10, 20, RED);
  if (score >= 10000) DrawText(TextFormat("SCORE: %i", score), 10, 65, 20, (Color){ 222, 181, 0, 255 });
  else DrawText(TextFormat("SCORE: %i", score), 10, 65, 20, BLUE);
  if (pause && ((pauseTimer/30)%2)) DrawText("PAUSED", 330, 190, 30, WHITE);
}

void UnloadGameplayScreen()
{
  UnloadSound(fxhit);
  UnloadSound(fxfeather);
  UnloadSound(fxboom);
  UnloadTexture(player_sprite);
  UnloadTexture(feather_sprite);
  UnloadTexture(enemy_sprite);
  UnloadTexture(firework_sprite);
  UnloadTexture(attack_sprite);
}

void gameReset(void)
{
   InitGameplayScreen();
   InitGameoverScreen();
}
