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
  DrawTextEx(ZadoBold, "CREDITS", (Vector2){ 290, 20 }, 50, 2, BLUE);
  DrawTextEx(ZadoBold, "Programming and Art by Return0ne", (Vector2){ 10, 160 }, 40, 2, BLUE);
  DrawTextEx(ZadoBold, "Powered by raylib 4.0", (Vector2){ 10, 200 }, 40, 2, BLUE);
  DrawTextEx(ZadoBold, "rFXgen used for sfx", (Vector2){ 10, 235 }, 40, 2, BLUE);
  DrawTextEx(ZadoBold, "A Canneddonuts project 2022", (Vector2){ 10, 270 }, 40, 2, BLUE);
  DrawTextEx(ZadoBold, TextFormat("Build compiled on %s", __DATE__), (Vector2){ 10, 310 }, 30, 2, GREEN);
  DrawTextEx(ZadoBold, "Press 'ENTER' ", (Vector2){ 10, 350 }, 30, 2, WHITE);
}

int FinishCreditsScreen(void)
{
  return finishfromCreditsScreen;
}

void UnloadCreditsScreen(void)
{

}
