/*
-    Avoid ~ a game by Canneddonuts
-      Filename ~ Credits.c
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#include "../include/raylib.h"

#include "Screens.h"
#include "Controls.h"
#include "Gfx.h"

int finishfromCreditsScreen = 0;

void InitCreditsScreen(void)
{
  finishfromCreditsScreen = 0;
}

void UpdateCreditsScreen(void)
{
  if (INPUT_OPTION_PRESSED) finishfromCreditsScreen = 1;
}

void DrawCreditsScreen(void)
{
  DrawTexture(background, 0, 0, DARKGRAY);
  DrawText("CREDITS", 290, 20, 50, BLUE);
  DrawText("Art by Mark and Allan", 10, 120, 40, BLUE);
  DrawText("Programming by Mark", 10, 160, 40, BLUE);
  DrawText("Powered by raylib 4.0", 10, 200, 40, BLUE);
  DrawText("rFXgen used for sfx", 10, 235, 40, BLUE);
  DrawText("A Canneddonuts project 2022", 10, 270, 40, BLUE);
  DrawText(TextFormat("Build compiled on %s", __DATE__),  10, 310, 30, GREEN);
  DrawText("Press 'ENTER' ", 10, 350, 30, WHITE);
}

int FinishCreditsScreen(void)
{
  return finishfromCreditsScreen;
}

void UnloadCreditsScreen(void)
{

}
