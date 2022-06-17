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

void UpdateCreditsScreen(void)
{
  if (INPUT_OPTION_PRESSED) currentScreen = TITLE;
}

void DrawCreditsScreen(void)
{
  DrawTexture(background, 0, 0, DARKGRAY);
  DrawText("CREDITS", 290, 20, 50, BLUE);
  DrawText("Programming and Art by Return0ne", 10, 210, 20, BLUE);
  DrawText("Powered by raylib 4.0", 10, 240, 20, BLUE);
  DrawText("A Canneddonuts project 2022", 10, 270, 40, BLUE);
  DrawText(TextFormat("Build compiled on %s", __DATE__), 10, 310, 30, YELLOW);
  DrawText("Press 'ENTER' ", 10, 350, 20, WHITE);
}
