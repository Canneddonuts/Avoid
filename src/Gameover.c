/*
-    Avoid ~ a game by Canneddonuts
-      Filename ~ Gameover.c
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#include "../include/raylib.h"

#include "Screens.h"
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
  DrawText("GAMEOVER", 250, 20, 50, RED);
  if (gameoverSelected == 0) DrawText("RETRY", 350, 200, 20, WHITE);
  else DrawText("RETRY", 350, 200, 20, RED);

  if (gameoverSelected == -1) DrawText("TITLE", 352, 230, 20, WHITE);
  else DrawText("TITLE", 352, 230, 20, RED);
}

int FinishGameoverScreen(void)
{
  return finishfromGameoverScreen;
}

void UnloadGameoverScreen(void)
{

}
