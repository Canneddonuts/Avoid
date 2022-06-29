/*
-    Avoid ~ a game by Canneddonuts
-        Filename ~ Main.c
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#include "../include/raylib.h"

#include "Screens.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

// screen variables
static const int screenWidth  = 800;
static const int screenHeight = 450;

GameScreen currentScreen = 0;

Texture2D background;
Texture2D player_sprite;
Texture2D feather_sprite;
Texture2D enemy_sprite;
Texture2D firework_sprite;

// Game functions
static void gameSetup(void);
static void updateGame(void);
static void drawGame(void);
static void gameLoop(void);
static void unloadGame(void);

int main(void)
{
  InitWindow(screenWidth, screenHeight, "Avoid");

  InitAudioDevice();

  gameSetup();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(gameLoop, 60, 1);
#else
    SetTargetFPS(60);

    while (!WindowShouldClose()) gameLoop();
#endif

  unloadGame();

  CloseAudioDevice();

  CloseWindow();

  return 0;
}

void gameSetup(void)
{
  // asset loading & setting of variable values
   currentScreen = TITLE;

   InitGameplayScreen();
   InitTitleScreen();
}

void updateGame(void)
{
  // code that runs as long as the program is running
 if ((IsKeyDown(KEY_LEFT_ALT)) && (IsKeyPressed(KEY_F))) ToggleFullscreen();

  switch (currentScreen) {
    case TITLE:  UpdateTitleScreen(); break;
    case GAMEPLAY: UpdateGameplayScreen(); break;
    case GAMEOVER: UpdateGameoverScreen(); break;
    case CREDITS: UpdateCreditsScreen(); break;
    default: break;
  }
}

void drawGame(void)
{
  // code to render the game to the game window
  BeginDrawing();

      ClearBackground(RAYWHITE);

      switch (currentScreen) {
        case TITLE: DrawTitleScreen(); break;
        case GAMEPLAY: DrawGameplayScreen(); break;
        case GAMEOVER: DrawGameoverScreen(); break;
        case CREDITS: DrawCreditsScreen(); break;
        default: break;
      }

  EndDrawing();
}

void gameLoop(void)
{
  updateGame();
  drawGame();
}

void unloadGame(void)
{
  UnloadGameplayScreen();
  UnloadTitleScreen();
}
