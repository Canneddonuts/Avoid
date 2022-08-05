/*
-    Avoid ~ a game by Canneddonuts
-      Filename ~ Ending.c
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#include "../include/raylib.h"

#include "Screens.h"
#include "Gfx.h"
#include "Controls.h"

int finishfromEndingScreen = 0;

void InitEndingScreen(void)
{
  finishfromEndingScreen = 0;
}

void UpdateEndingScreen(void)
{
  if (INPUT_OPTION_PRESSED) finishfromEndingScreen = 1;
}

void DrawEndingScreen(void)
{
  DrawTexture(background, 0, 0, GOLD);
  DrawTextEx(ZadoBold, "THANK YOU SO MUCH FOR PLAYING!!!", (Vector2){ 10, 10 }, 45, 2, GOLD);
  DrawTextEx(ZadoBold, "Canneddonuts 2022", (Vector2){ 380, 400 }, 40, 2, WHITE);
  DrawTextEx(ZadoBold, "Press 'ENTER'", (Vector2){ 0, 400 }, 40, 2, WHITE);
}

void UnloadEndingScreen(void)
{
}

int FinishEndingScreen(void)
{
  return finishfromEndingScreen;
}
