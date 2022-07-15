/*
-    Avoid ~ a game by Canneddonuts
-      Filename ~ Title.h
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#include "../include/raylib.h"

#include "Screens.h"
#include "Controls.h"
#include "Score.h"
#include "Textures.h"

int titleSelected = 0;

void DrawScore(void)
{
  if (bestscore >= 10000)
     DrawText(TextFormat("BEST: %i", bestscore), 600, 0, 30, (Color){ 222, 181, 0, 255 });
  else if (bestscore >= 5000)
     DrawText(TextFormat("BEST: %i", bestscore), 600, 0, 30, (Color){ 149, 148, 147, 255 });
  else if (bestscore >= 1000)
     DrawText(TextFormat("BEST: %i", bestscore), 600, 0, 30, (Color){ 138, 72, 4, 255 });
  else
     DrawText(TextFormat("BEST: %i", bestscore), 600, 0, 30, BLUE);
}

void InitTitleScreen(void)
{
  background = LoadTexture("assets/gfx/background.png");
}

void UpdateTitleScreen(void)
{
  if (INPUT_UP_PRESSED) titleSelected++;
  if (INPUT_DOWN_PRESSED) titleSelected--;
  if (titleSelected > 0) titleSelected--;
  if (titleSelected < -2) titleSelected++;

  if ((titleSelected == 0) && (INPUT_OPTION_PRESSED)) currentScreen = GAMEPLAY;
  if ((titleSelected == -1) && (INPUT_OPTION_PRESSED)) currentScreen = CREDITS;
  if ((titleSelected == -2) && (INPUT_OPTION_PRESSED)) OpenURL("https://canneddonuts.itch.io/");
}

void DrawTitleScreen(void)
{
  DrawTexture(background, 0, 0, GRAY);
  DrawText("Controls", 10, 10, 30, BLUE);
  DrawScore();
  DrawText("Press the arrow keys or 'DPAD' to move and 'X' to dash", 10, 40, 10, WHITE);
  DrawText("Press 'ENTER' or 'START' to pause", 10, 60, 10, WHITE);
  DrawText("Press 'M' to mute", 10, 80, 10, WHITE);
  DrawText("Press 'Left-ALT' + 'F' for full screen", 10, 100, 10, WHITE);
  DrawText("Press 'R' to restart", 10, 120, 10, WHITE);
  DrawText("Press 'ENTER' or 'START' to select an option", 10, 140, 10, WHITE);
  DrawText("Press 'X' or 'A' on a gamepad to shoot", 10, 160, 10, WHITE);
  DrawText("Avoid", 330, 20, 50, BLUE);
//  DrawText("Ver: 0.1", 680, 420, 30, WHITE);
  if (titleSelected == 0) DrawText("PLAY", 360, 220, 20, WHITE);
  else DrawText("PLAY", 360, 220, 20, BLUE);

  if (titleSelected == -1) DrawText("CREDITS", 340, 240, 20, WHITE);
  else DrawText("CREDITS", 340, 240, 20, BLUE);

  if (titleSelected == -2) DrawText("MORE GAMES", 320, 260, 20, WHITE);
  else DrawText("MORE GAMES", 320, 260, 20, BLUE);
}

void UnloadTitleScreen(void)
{
  UnloadTexture(background);
}
