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

void UpdateCreditsScreen(void)
{
  if (INPUT_OPTION_PRESSED) currentScreen = TITLE;
}

void DrawCreditsScreen(void)
{
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GREEN);

  DrawText("Avoid", 330, 20, 50, MAGENTA);

  DrawText("Programming and Art by Return0ne", 10, 210, 20, BLUE);

  DrawText("Powered by raylib 4.0", 10, 240, 20, BLUE);

  DrawText("A Canneddonuts project 2022", 10, 270, 40, RED);

  DrawText("Press 'ENTER' ", 10, 350, 20, WHITE);
}
