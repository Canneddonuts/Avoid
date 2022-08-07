/*
-    Avoid ~ a game by Canneddonuts
-      Filename ~ Options.c
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#include "../include/raylib.h"

#include "Screens.h"
#include "Gfx.h"
#include "Controls.h"
#include "Options.h"

int optionsSelected = 0, finishfromOptionsScreen = 0, mute = 0, fullscreen = 0;

void InitOptionsScreen(void)
{
  finishfromOptionsScreen = 0;
  optionsSelected = 0;
}

void UpdateOptionsScreen(void)
{
  if (INPUT_UP_PRESSED) optionsSelected++;
  if (INPUT_DOWN_PRESSED) optionsSelected--;
  if (optionsSelected > 0) optionsSelected--;
  if (optionsSelected < -2) optionsSelected++;

  if ((optionsSelected == 0) && (INPUT_OPTION_PRESSED)) finishfromOptionsScreen = 1;
  if ((optionsSelected == -1) && (INPUT_OPTION_PRESSED)) mute = !mute;
  if ((optionsSelected == -2) && (INPUT_OPTION_PRESSED)) { ToggleFullscreen(); fullscreen = !fullscreen; }
}

void DrawOptionsScreen(void)
{
  DrawTexture(background, 0, 0, DARKGRAY);
  DrawTextEx(ZadoBold, "OPTIONS", (Vector2){ 300, 20 }, 50, 2, BLUE);
  if (optionsSelected == 0) DrawTextEx(ZadoBold, "Back", (Vector2){ 20, 170 }, 40, 2, WHITE);
  else DrawTextEx(ZadoBold, "Back", (Vector2){ 20, 170 }, 40, 2, BLUE);
  if (optionsSelected == -1) {
    DrawTextEx(ZadoBold, "Mute", (Vector2) { 20, 220 }, 40, 2, WHITE);
    DrawTextEx(ZadoBold, TextFormat("<%i>", mute), (Vector2){ 250, 220 }, 40, 2, WHITE);
  }
  else {
    DrawTextEx(ZadoBold, "Mute", (Vector2) { 20, 220 }, 40, 2, BLUE);
    DrawTextEx(ZadoBold, TextFormat("<%i>", mute), (Vector2){ 250, 220 }, 40, 2, BLUE);
  }
  if (optionsSelected == -2) {
    DrawTextEx(ZadoBold, "Fullscreen", (Vector2){ 20, 270 }, 40, 2, WHITE);
    DrawTextEx(ZadoBold, TextFormat("<%i>", fullscreen), (Vector2){ 250, 270 }, 40, 2, WHITE);
  }
  else {
    DrawTextEx(ZadoBold, "Fullscreen", (Vector2){ 20, 270 }, 40, 2, BLUE);
    DrawTextEx(ZadoBold, TextFormat("<%i>", fullscreen), (Vector2){ 250, 270 }, 40, 2, BLUE);
  }
}

void UnloadOptionsScreen(void)
{

}

int FinishOptionsScreen(void)
{
  return finishfromOptionsScreen;
}
