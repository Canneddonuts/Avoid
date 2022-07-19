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
#include "Textures.h"

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
  DrawText("Programming and Art by Return0ne", 10, 210, 20, BLUE);
  DrawText("Powered by raylib 4.0 and rFXgen for sound effects", 10, 240, 20, BLUE);
  DrawText("A Canneddonuts project 2022", 10, 270, 40, BLUE);
  DrawText(TextFormat("Build compiled on %s", __DATE__), 10, 310, 30, GREEN);
  DrawText("Press 'ENTER' ", 10, 350, 20, WHITE);
}

int FinishCreditsScreen(void)
{
  return finishfromCreditsScreen;
}

void UnloadCreditsScreen(void)
{

}
