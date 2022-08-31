/*
-    Avoid ~ a game by Canneddonuts
-      Filename ~ Gameover.c
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#include "../include/raylib.h"

#include "Screens.h"
#include "Options.h"
#include "Controls.h"
#include "Music.h"
#include "Gfx.h"

int gameoverSelected = 0, finishfromGameoverScreen = 0;
Music Gameoversong = { 0 };

void InitGameoverScreen(void)
{
  gameoverSelected = 0;
  finishfromGameoverScreen = 0;
  Gameoversong = LoadMusicStream("assets/bgm/02-Have-Hope.ogg");
  PlayMusicStream(Gameoversong);
}

void UpdateGameoverScreen(void)
{
  if (!mute) UpdateMusicStream(Gameoversong);
  if (INPUT_UP_PRESSED) gameoverSelected++;
  if (INPUT_DOWN_PRESSED) gameoverSelected--;
  if (gameoverSelected > 0) gameoverSelected--;
  if (gameoverSelected < -1) gameoverSelected++;

  if ((gameoverSelected == 0) && (INPUT_OPTION_PRESSED))
    { StopMusicStream(Gameoversong); finishfromGameoverScreen = 2; }

  if ((gameoverSelected == -1) && (INPUT_OPTION_PRESSED))
    { StopMusicStream(Gameoversong); finishfromGameoverScreen = 1; }
}

void DrawGameoverScreen(void)
{
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
  DrawText("GAME OVER", 170, 10, 80, RED);
  if (gameoverSelected == 0) DrawTextEx(ZadoBold, "RETRY", (Vector2){ 340, 200 }, 40, 2, WHITE);
  else DrawTextEx(ZadoBold, "RETRY", (Vector2){ 340, 200 }, 40, 2, RED);

  if (gameoverSelected == -1) DrawTextEx(ZadoBold, "TITLE", (Vector2){ 345, 250 }, 40, 2, WHITE);
  else DrawTextEx(ZadoBold, "TITLE", (Vector2){ 345, 250 }, 40, 2, RED);
}

int FinishGameoverScreen(void)
{
  return finishfromGameoverScreen;
}

void UnloadGameoverScreen(void)
{
  UnloadMusicStream(Gameoversong);
}
