/*
-    Avoid ~ a game by Canneddonuts
-      Filename ~ Ending.c
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#include "../include/raylib.h"

#include "Screens.h"
#include "Textures.h"
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
  DrawText("THANK YOU SO MUCH FOR PLAYING!!!", 145, 20, 30, GOLD);
  DrawText("Canneddonuts 2022", 500, 420, 30, WHITE);
  DrawText("Press 'ENTER'", 0, 420, 30, WHITE);
}

void UnloadEndingScreen(void)
{
}

int FinishEndingScreen(void)
{
  return finishfromEndingScreen;
}
