/*
-    Avoid ~ a game by Canneddonuts
-      Filename ~ LevelSel.c
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#include "../include/raylib.h"

#include "Screens.h"
#include "Stats.h"
#include "Controls.h"
#include "Gfx.h"

int finishfromLevelSelScreen = 0, levelSelected = 1, levelunlocked = 0;

void InitLevelSelScreen(void)
{
  feather_sprite = LoadTexture("assets/gfx/feather.png");
  finishfromLevelSelScreen = 0;
}

void UpdateLevelSelScreen(void)
{
  if (INPUT_LEFT_PRESSED) levelSelected--;
  if (INPUT_RIGHT_PRESSED) if (levelunlocked >= levelSelected) levelSelected++;
  if (levelSelected < 1) levelSelected++;
  if (levelSelected > 3) levelSelected--;

  if ((levelSelected == 1) && (INPUT_OPTION_PRESSED)) { level = LEVEL1; finishfromLevelSelScreen = 1; }
  if ((levelSelected == 2) && (INPUT_OPTION_PRESSED)) { level = LEVEL2; finishfromLevelSelScreen = 1; }
  if ((levelSelected == 3) && (INPUT_OPTION_PRESSED)) { level = LEVEL3; finishfromLevelSelScreen = 1; }
}

void DrawLevelSelScreen(void)
{
  DrawTexture(background, 0, 0, GRAY);

  DrawTexture(feather_sprite, 0, 0, GREEN);
  DrawText(TextFormat("= %i", greenfeathers), 30, 30, 30, GREEN);
  DrawTexture(feather_sprite, 80, 0, RED);
  DrawText(TextFormat("= %i", redfeathers), 110, 30, 30, RED);
  if (score >= 10000) DrawText(TextFormat("SCORE: %i", score), 10, 65, 30, (Color){ 222, 181, 0, 255 });
  else DrawText(TextFormat("SCORE: %i", score), 10, 65, 30, BLUE);

  if (levelSelected == 1) DrawText("1", 100, 220, 30, WHITE);
  else DrawText("1", 100, 220, 30, BLUE);

  if (levelSelected == 2) DrawText("2", 150, 220, 30, WHITE);
  else if (levelunlocked >= 1) DrawText("2", 150, 220, 30, BLUE);
  else DrawText("2", 150, 220, 30, GRAY);

  if (levelSelected == 3) DrawText("3", 200, 220, 30, WHITE);
  else  if (levelunlocked >= 2) DrawText("3", 200, 220, 30, BLUE);
  else  DrawText("3", 200, 220, 30, GRAY);
}

void UnloadLevelSelScreen(void)
{
  UnloadTexture(feather_sprite);
}

int FinishLevelSelScreen(void)
{
  return finishfromLevelSelScreen;
}
