/*
-    Avoid ~ a game by Canneddonuts
-        Filename ~ Main.c
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#include "../include/raylib.h"

#include "Screens.h"
#include "Options.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

// screen variables
static const int screenWidth  = 800;
static const int screenHeight = 450;

static float transAlpha = 0.0f;
static bool onTransition = false;
static bool transFadeOut = false;
static int transFromScreen = -1;
static int transToScreen = -1;

GameScreen currentScreen = 0;
Levels level = 0;

Texture2D background;
Texture2D player_sprite;
Texture2D feather_sprite;
Texture2D enemy_sprite;
Texture2D firework_sprite;
Texture2D attack_sprite;
Font ZadoBold;

// Game functions
static void gameSetup(void);
static void update_draw_frame(void);
static void unloadGame(void);
static void transition_to_screen(int screen);
static void update_transition(void);
static void draw_transition(void);

int main(void)
{
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(screenWidth, screenHeight, "Avoid");

  InitAudioDevice();

  gameSetup();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(update_draw_frame, 60, 1);
#else
    SetTargetFPS(60);

    while (!WindowShouldClose()) update_draw_frame();
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
   level = LEVEL1;
   background = LoadTexture("assets/gfx/background.png");
   ZadoBold = LoadFontEx("assets/fonts/ZadoBold.ttf", 96, 0, 110);

   SetMasterVolume(0.5);
}


static void transition_to_screen(int screen)
{
  onTransition = true;
  transFadeOut = false;
  transFromScreen = currentScreen;
  transToScreen = screen;
  transAlpha = 0.0f;
}

static void update_transition(void)
{
  if (!transFadeOut) {
    transAlpha += GetFrameTime();

    if (transAlpha > 0.76f) {
      transAlpha = 0.75f;

      switch (transFromScreen) {
        case TITLE: UnloadTitleScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        case GAMEOVER: UnloadGameoverScreen(); break;
        case CREDITS: UnloadCreditsScreen(); break;
        case OPTIONS: UnloadOptionsScreen(); break;
        case ENDING: UnloadEndingScreen(); break;
        case LEVELSEL: UnloadLevelSelScreen(); break;
        default: break;
      }

      switch (transToScreen) {
        case TITLE:  InitTitleScreen(); break;
        case GAMEPLAY: LoadGamplayScreen(); InitGameplayScreen(); break;
        case GAMEOVER: InitGameoverScreen(); break;
        case CREDITS: InitCreditsScreen(); break;
        case OPTIONS: InitOptionsScreen(); break;
        case ENDING: InitEndingScreen(); break;
        case LEVELSEL: InitLevelSelScreen(); break;
        default: break;
      }

      currentScreen = transToScreen;

      transFadeOut = true;
    }
  } else {
    transAlpha -= GetFrameTime();

    if (transAlpha < -0.75f) {
      transAlpha = 0.0f;
      transFadeOut = false;
      onTransition = false;
      transFromScreen = -1;
      transToScreen = -1;
    }
  }
}

static void draw_transition(void)
{
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
}


static void update_draw_frame(void)
{
  if (IsKeyPressed(KEY_M)) mute = !mute;
  if ((IsKeyDown(KEY_LEFT_ALT)) && (IsKeyPressed(KEY_F))) { ToggleFullscreen(); fullscreen = !fullscreen; }
//  printf("%f\n", GetFrameTime());
//  printf("%f\n", transAlpha);

  if (!onTransition) {
    switch (currentScreen) {
      case TITLE: {
        UpdateTitleScreen();

        switch (FinishTitleScreen()) {
          case 1: transition_to_screen(CREDITS); break;
          case 2: transition_to_screen(LEVELSEL); break;
          case 3: transition_to_screen(OPTIONS); break;
        }
      } break;
      case CREDITS: {
        UpdateCreditsScreen();

        if (FinishCreditsScreen() == 1) transition_to_screen(TITLE);
      } break;
      case GAMEPLAY: {
        UpdateGameplayScreen();

        switch (FinishGameplayScreen()) {
          case 1: transition_to_screen(GAMEOVER); break;
          case 2: transition_to_screen(TITLE); break;
          case 3: transition_to_screen(ENDING); break;
          case 4: transition_to_screen(LEVELSEL); break;
        }
      } break;
      case GAMEOVER: {
        UpdateGameoverScreen();

        if (FinishGameoverScreen() == 1) transition_to_screen(TITLE);
        else if (FinishGameoverScreen() == 2) transition_to_screen(GAMEPLAY);
      } break;
      case OPTIONS: {
        UpdateOptionsScreen();

        if (FinishOptionsScreen() == 1) transition_to_screen(TITLE);
      } break;
      case ENDING: {
        UpdateEndingScreen();

        if (FinishEndingScreen() == 1) transition_to_screen(TITLE);
      } break;
      case LEVELSEL: {
        UpdateLevelSelScreen();

        if (FinishLevelSelScreen() == 1) transition_to_screen(GAMEPLAY);
      } break;
      default: break;
    }
  } else update_transition();

  BeginDrawing();

     ClearBackground(RAYWHITE);

     switch (currentScreen) {
       case TITLE: DrawTitleScreen(); break;
       case CREDITS: DrawCreditsScreen(); break;
       case GAMEPLAY: DrawGameplayScreen(); break;
       case GAMEOVER: DrawGameoverScreen(); break;
       case OPTIONS: DrawOptionsScreen(); break;
       case ENDING: DrawEndingScreen(); break;
       case LEVELSEL: DrawLevelSelScreen(); break;
       default: break;
     }

    // DrawText(TextFormat("GetTime(): %f", GetTime()), 10, 320, 20, GREEN);

     if (onTransition) draw_transition();

  EndDrawing();
}

static void unloadGame(void)
{
    switch (currentScreen) {
      case TITLE: UnloadTitleScreen(); break;
      case GAMEPLAY: UnloadGameplayScreen(); break;
      case GAMEOVER: UnloadGameoverScreen(); break;
      case CREDITS: UnloadCreditsScreen(); break;
      case OPTIONS: UnloadOptionsScreen(); break;
      case ENDING: UnloadEndingScreen(); break;
      case LEVELSEL: UnloadLevelSelScreen(); break;
      default: break;
    }

    UnloadFont(ZadoBold);
    UnloadTexture(background);
}
