/*
-    Avoid ~ a game by Canneddonuts
-      Filename ~ Gameover.c
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#include "../include/raylib.h"

#include "Screens.h"
#include "Gfx.h"
#include "Controls.h"

int gameoverSelected = 0, finishfromGameoverScreen = 0;

void InitGameoverScreen(void)
{
  gameoverSelected = 0;
  finishfromGameoverScreen = 0;
}

void UpdateGameoverScreen(void)
{
  if (INPUT_UP_PRESSED) gameoverSelected++;
  if (INPUT_DOWN_PRESSED) gameoverSelected--;
  if (gameoverSelected > 0) gameoverSelected--;
  if (gameoverSelected < -1) gameoverSelected++;

  if ((gameoverSelected == 0) && (INPUT_OPTION_PRESSED))
    finishfromGameoverScreen = 2;

  if ((gameoverSelected == -1) && (INPUT_OPTION_PRESSED))
    finishfromGameoverScreen = 1;
}

void DrawGameoverScreen(void)
{
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
  DrawTextEx(ZadoBold, "GAMEOVER", (Vector2){ 220, 20 }, 80, 3, RED);
  if (gameoverSelected == 0) DrawTextEx(ZadoBold, "RETRY", (Vector2){ 350, 200 }, 30, 2, WHITE);
  else DrawTextEx(ZadoBold, "RETRY", (Vector2){ 350, 200 }, 30, 2, RED);

  if (gameoverSelected == -1) DrawTextEx(ZadoBold, "TITLE", (Vector2){ 355, 230 }, 30, 2, WHITE);
  else DrawTextEx(ZadoBold, "TITLE", (Vector2){ 355, 230 }, 30, 2, RED);
}

int FinishGameoverScreen(void)
{
  return finishfromGameoverScreen;
}

void UnloadGameoverScreen(void)
{

}
