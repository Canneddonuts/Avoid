/*
-    Avoid ~ a game by Canneddonuts
-      Filename ~ Ending.c
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#include "../include/raylib.h"

#include "Screens.h"
#include "Controls.h"
#include "Options.h"
#include "Music.h"
#include "Gfx.h"

int finishfromEndingScreen = 0;
Music Endingsong = { 0 };

void InitEndingScreen(void)
{
  finishfromEndingScreen = 0;
  Endingsong = LoadMusicStream("assets/bgm/01-Slipin-Sunday.ogg");
  PlayMusicStream(Endingsong);
}

void UpdateEndingScreen(void)
{
  if (!mute) UpdateMusicStream(Endingsong);
  if (INPUT_OPTION_PRESSED) finishfromEndingScreen = 1;
}

void DrawEndingScreen(void)
{
  DrawTexture(background, 0, 0, GOLD);
  DrawTextEx(ZadoBold, "THANK YOU SO MUCH FOR PLAYING!!!", (Vector2){ 10, 10 }, 45, 2, GOLD);
  DrawTextEx(ZadoBold, "Canneddonuts 2022", (Vector2){ 380, 400 }, 40, 2, WHITE);
  DrawTextEx(ZadoBold, "Press 'ENTER'", (Vector2){ 5, 400 }, 40, 2, WHITE);
}

void UnloadEndingScreen(void)
{
  UnloadMusicStream(Endingsong);
}

int FinishEndingScreen(void)
{
  return finishfromEndingScreen;
}
