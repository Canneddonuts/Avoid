/*
-    Avoid ~ a game by Canneddonuts
-      Filename ~ Title.c
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#include "../include/raylib.h"

#include "Screens.h"
#include "Controls.h"
#include "Music.h"
#include "Options.h"
#include "Gfx.h"

int titleSelected = 0, finishfromTitleScreen = 0;
Music Titlesong = { 0 };

void InitTitleScreen(void)
{
  titleSelected = 0;
  finishfromTitleScreen = 0;
  Titlesong = LoadMusicStream("assets/bgm/04-Distant-Misadventure.ogg");
  PlayMusicStream(Titlesong);
}

void UpdateTitleScreen(void)
{
  if (!mute) UpdateMusicStream(Titlesong);
  if (INPUT_UP_PRESSED) titleSelected++;
  if (INPUT_DOWN_PRESSED) titleSelected--;
  if (titleSelected > 0) titleSelected--;
  if (titleSelected < -2) titleSelected++;

  if ((titleSelected == 0) && (INPUT_OPTION_PRESSED))  { StopMusicStream(Titlesong); finishfromTitleScreen = 2; }
  if ((titleSelected == -1) && (INPUT_OPTION_PRESSED)) { StopMusicStream(Titlesong); finishfromTitleScreen = 1; }
  if ((titleSelected == -2) && (INPUT_OPTION_PRESSED)) { StopMusicStream(Titlesong); finishfromTitleScreen = 3; }
}

void DrawTitleScreen(void)
{
  DrawTexture(background, 0, 0, GRAY);
  DrawTextEx(ZadoBold, "Avoid", (Vector2){ 300, 0 }, 80, 5, BLUE);
    DrawText("Controls", 5, 10, 30, BLUE);
    DrawText("Press the arrow keys or 'DPAD' to move and 'X' to dash", 5, 40, 10, WHITE);
    DrawText("Press 'ENTER' or 'START' to pause", 5, 60, 10, WHITE);
    DrawText("Press 'M' to mute", 5, 80, 10, WHITE);
    DrawText("Press 'Left-ALT' + 'F' for full screen", 5, 100, 10, WHITE);
    DrawText("Press 'R' to restart", 5, 120, 10, WHITE);
    DrawText("Press 'ENTER' or 'START' to select an option", 5, 140, 10, WHITE);
    DrawText("Press 'Z' or 'A' on a gamepad to shoot", 5, 160, 10, WHITE);
    DrawText("Press 'Q' to return to level select", 5, 180, 10, WHITE);
    DrawText("Remember to shoot down the fireworks.", 5, 200, 10, RED);
//  DrawText("Ver: 0.1", 680, 420, 30, WHITE);
    if (titleSelected == 0) DrawTextEx(ZadoBold,"PLAY", (Vector2){ 360, 220 }, 30, 2, WHITE);
    else DrawTextEx(ZadoBold,"PLAY", (Vector2){ 360, 220 }, 30, 2, BLUE);

    if (titleSelected == -1) DrawTextEx(ZadoBold, "CREDITS", (Vector2){ 330, 250 }, 30, 2, WHITE);
    else DrawTextEx(ZadoBold, "CREDITS", (Vector2){ 330, 250 }, 30, 2, BLUE);

    if (titleSelected == -2) DrawTextEx(ZadoBold, "OPTIONS", (Vector2){ 330, 280 }, 30, 2, WHITE);
    else DrawTextEx(ZadoBold, "OPTIONS", (Vector2){ 330, 280 }, 30, 2, BLUE);
}

void UnloadTitleScreen(void)
{
  UnloadMusicStream(Titlesong);
}

int FinishTitleScreen(void)
{
  return finishfromTitleScreen;
}
