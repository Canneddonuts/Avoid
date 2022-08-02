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
#include "Options.h"
#include "Gameplay.h"
#include "Score.h"
#include "Timers.h"
#include "Textures.h"

int score = 0, bestscore = 0, finishfromGameplayScreen = 0;
Levels level = 0;

void SetEnemyLevel(void)
{
  switch (level) {
    case LEVEL1: enemy.speed *= 1.0f; break;
    case LEVEL2: enemy.speed *= 2.0f; break;
    case LEVEL3: enemy.speed *= 2.0f; break;
  }
}

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
  finishfromGameplayScreen = 0;

  level = LEVEL1;

  globalTimer = 0;

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
  player.iframetimer = 0;
  player.in = false;
  player.color = RAYWHITE;

  enemy.currentframe = 0;
  enemy.hp = 5;
  enemy.speed = 2.0f;
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
  enemy.in = false;
  enemy.iframetimer = 0;

  feather.hitbox = (Rectangle) {
    GetRandomValue(0, GetScreenWidth() - feather_sprite.width),
    GetRandomValue(0, GetScreenHeight() - feather_sprite.height),
    (float) feather_sprite.width,
    (float) feather_sprite.height
  };
  feather.active = true;
  feather.power = 0;

  for (int i = 0; i < MAX_FIREWORKS; i++) {
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
    shoot[i].speed.x = 500.f;
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
  if (!player.in) {
    player.hp--;
    if (!mute) PlaySoundMulti(fxhit);
    player.in = true;
  }

  player.currentframe = 1;
}

void UpdateiFrameTimer(struct Actor* actor)
{
  // here we use pointers to avoid duplicating code
  if (actor->in) {
    actor->iframetimer++;
    actor->currentframe = 1;
    if (globalTimer % 2 == 0) actor->color = BLANK;
    else actor->color = RAYWHITE;
    if (actor->iframetimer >= 60) {
      actor->in = false;
      actor->iframetimer = 0;
    }
  } else { actor->color = RAYWHITE; actor->currentframe = 0; }
}

void UpdateGameplayScreen(void)
{
   if (INPUT_OPTION_PRESSED) pause = !pause;
   // code to end the game
   if (level > 2) finishfromGameplayScreen = 3;

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
         score++;
         globalTimer++;
         // pass the address of each struct to the UpdateiFrameTimer function
         UpdateiFrameTimer(&player);
         UpdateiFrameTimer(&enemy);
         if (score >= bestscore)  bestscore = score;

         // Debug stuff
         if (IsKeyPressed(KEY_D)) DebugMode = !DebugMode;
         if (IsKeyPressed(KEY_NINE)) ammo = 99;
         if (IsKeyPressed(KEY_ZERO)) ammo = 0;
         if (IsKeyPressed(KEY_R)) finishfromGameplayScreen = 2;
         if (IsKeyPressed(KEY_W)) finishfromGameplayScreen = 3;

         // call gameover when killed
         if (player.hp <= 0) finishfromGameplayScreen = 1;

         // Red feather logic
         for (int i = 0; i < MAX_SHOOTS; i++) {
           if (shoot[i].active) {
             shoot[i].hitbox.x += shoot[i].speed.x * GetFrameTime();
           }

           if (CheckCollisionRecs(shoot[i].hitbox, enemy.hitbox) && shoot[i].active) {
             if (!enemy.in) enemy.hp--;
             enemy.in = true;
             score += 300;
             if (!mute) PlaySoundMulti(fxboom);
             shoot[i].active = false;
           }

           if (shoot[i].hitbox.x + shoot[i].hitbox.width >= GetScreenWidth() + attack_sprite.width) shoot[i].active = false;
         }

          // Feather spawn logic
          switch (feather.power) {
             case 0:  feather.color = GREEN; break;
             case 1:  feather.color = RED; break;
           }
             if (CheckCollisionRecs(player.hitbox,  feather.hitbox)) {
                 switch (feather.power) {
                   case 0: player.hp++; feather.power = 1; break;
                   case 1: ammo++; if (player.hp < 5) feather.power = 0; else feather.power = 1; break;
                 }
                 if (!mute && player.hp < 5) PlaySoundMulti(fxfeather);
                 feather.hitbox.x = GetRandomValue(0, 600);
                 feather.hitbox.y = GetRandomValue(0, GetScreenHeight() - feather_sprite.height);
             }

         // Enemy logic
         if (level < 3) {
           if (((enemy.hitbox.y + enemy.hitbox.height) >= GetScreenHeight()
           || (enemy.hitbox.y <= 0))) enemy.speed *= -1.0f;

           enemy.hitbox.y += enemy.speed;

           if (CheckCollisionRecs(player.hitbox, enemy.hitbox)) DamagePlayer();

           if (enemy.hp < 1) { level++; enemy.hp = 5; SetEnemyLevel(); }
         }

         // Firework logic
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
                // Firework wall collision
                if (((fireworks[i].hitbox.x + -firework_sprite.width) > GetScreenWidth()
                || (fireworks[i].hitbox.x <= -firework_sprite.width))) fireworks[i].active = 0;
                break;
            }
           switch (level) {
             case LEVEL1: fireworks[i].speed.x = 300.0f; break;
             case LEVEL2: fireworks[i].speed.x = 600.0f; break;
             case LEVEL3: fireworks[i].speed.x = 900.0f; break;
           }
         }
       } else pauseTimer++;
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
    DrawRectangleRec(player.hitbox, BLUE);
    DrawRectangleRec(feather.hitbox, WHITE);
    DrawRectangleRec(enemy.hitbox, BLACK);
    for (int i = 0; i < MAX_FIREWORKS; i++) {
      DrawRectangleRec(fireworks[i].hitbox, BLACK);
    }
    for (int i = 0; i < MAX_SHOOTS; i++) {
      DrawRectangleRec(shoot[i].hitbox, GREEN);
    }
    DrawText(TextFormat("enemy.hitbox.y: %f", enemy.hitbox.y), 10, 200, 20, GREEN);
    DrawText(TextFormat("enemy.speed: %f", enemy.speed), 10, 220, 20, GREEN);
    DrawText(TextFormat("globalTimer: %i", globalTimer), 10, 240, 20, GREEN);
    DrawText(TextFormat("firework_sprite.width: %d", firework_sprite.width), 10, 260, 20, GREEN);
    DrawText(TextFormat("player.iframetimer: %d", player.iframetimer), 10, 280, 20, GREEN);
    DrawText(TextFormat("player.in: %d", player.in), 10, 300, 20, GREEN);
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
  DrawText(TextFormat("ENEMY HP: %i", enemy.hp), GetScreenWidth() - 150, 10, 20, RED);
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

int FinishGameplayScreen(void)
{
  return finishfromGameplayScreen;
}
