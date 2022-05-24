/*
-    Avoid a game by Canneddonuts
-        Filename ~ Main.c
-       Author ~ Return0ne
-            2022
-         *no license*
*/
#include "../include/raylib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

// screen variables
static const int screenWidth  = 800;
static const int screenHeight = 450;

// Gamescreens
typedef enum GameScreen { TITLE = 0, GAMEPLAY, GAMEOVER, CREDITS } GameScreen;

// structs
typedef struct Ball {
    Vector2 position;
    Vector2 speed;
    float radius;
    float growth;
    Color color;
    bool active;
} Ball;

typedef struct Player {
    Texture2D sprite;
    int currentframe;
    Vector2 sprite_pos;
    Rectangle frameRec;
    Rectangle hitbox;
    int hp;
} Player;

// Game variables
static int pauseTimer;
static int score, bestscore;
static int titleSelected = 0, gameoverSelected = 0;
static GameScreen currentScreen = { 0 };
static Sound fxbounce = { 0 };
static Player player = { 0 };
static Ball ball = { 0 };
static bool pause;
static bool mute;
static bool ShowHitbox;

// Game functions
static void gameSetup(void);
static void updateGame(void);
static void drawGame(void);
static void gameReset(void);
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

  CloseWindow();

  return 0;
}

void gameSetup(void)
{
  // asset loading & setting of variable values
   currentScreen = TITLE;

   fxbounce = LoadSound("assets/sfx/boing.wav");

   SetMasterVolume(0.2);

   player.sprite = LoadTexture("assets/gfx/player.png");
   player.currentframe = 0;
   player.hp = 30;
   player.frameRec = (Rectangle) {
     0.0f,
     0.0f,
    (float) player.sprite.width/2,
    (float) player.sprite.height
   };
   player.hitbox = (Rectangle) {
     GetScreenWidth()/2.0f - 30,
     GetScreenHeight()/2.0f - 30,
     70,
     70
   };

   ball.position = (Vector2){ 50, 50 };
   ball.speed = (Vector2){ 400.0f,  300.0f };
   ball.radius = 20;
   ball.growth = 2;
   ball.color = MAROON;
   ball.active = true;

   pause = 0;
   mute = 0;
   ShowHitbox = 0;
   pauseTimer = 0;
   score = 0;
}

void updateGame(void)
{
  // code that runs as long as the program is running
 if ((IsKeyDown(KEY_LEFT_ALT)) && (IsKeyPressed(KEY_F))) ToggleFullscreen();

  switch(currentScreen) {
    case TITLE:
        if (IsKeyPressed(KEY_UP)) titleSelected++;
        if (IsKeyPressed(KEY_DOWN)) titleSelected--;
        if (titleSelected > 0) titleSelected--;
        if (titleSelected < -2) titleSelected++;

        if ((titleSelected == 0) && (IsKeyPressed(KEY_ENTER))) currentScreen = GAMEPLAY;
        if ((titleSelected == -1) && (IsKeyPressed(KEY_ENTER))) currentScreen = CREDITS;
        if ((titleSelected == -2) && (IsKeyPressed(KEY_ENTER))) OpenURL("https://canneddonuts.itch.io/");
     break;
    case GAMEPLAY:

     if (IsKeyPressed(KEY_M)) mute = !mute;

    if (IsKeyPressed(KEY_ENTER)) pause = !pause;

     if (!pause) {
       // Controls
       if (IsKeyDown(KEY_LEFT)) player.hitbox.x -= GetFrameTime() * 300.0f;
       if (IsKeyDown(KEY_RIGHT)) player.hitbox.x += GetFrameTime() * 300.0f;
       if (IsKeyDown(KEY_UP)) player.hitbox.y -= GetFrameTime() * 300.0f;
       if (IsKeyDown(KEY_DOWN)) player.hitbox.y += GetFrameTime() * 300.0f;

       player.sprite_pos = (Vector2){ player.hitbox.x, player.hitbox.y };
       player.frameRec.x = (float)player.currentframe*(float)player.sprite.width/2;

       // Player to da wallz collies
       if ((player.hitbox.x + player.hitbox.width) >= GetScreenWidth()) player.hitbox.x = GetScreenWidth() - player.hitbox.width;
       else if (player.hitbox.x <= 0) player.hitbox.x = 0;

       if ((player.hitbox.y + player.hitbox.height) >= GetScreenHeight()) player.hitbox.y = GetScreenHeight() - player.hitbox.height;
       else if (player.hitbox.y <= 0) player.hitbox.y = 0;

       if (IsKeyPressed(KEY_D)) ball.active = !ball.active;

       if (IsKeyPressed(KEY_H)) ShowHitbox = !ShowHitbox;

       if (IsKeyPressed(KEY_R)) {
         gameReset();
         currentScreen = TITLE;
       }

       if (player.hp <= 0) {
         gameReset();
         currentScreen = GAMEOVER;
       }

        if (ball.active) {
          score++;
          // moveiement of the balls
          ball.position.x += GetFrameTime() * ball.speed.x;
          ball.position.y += GetFrameTime() * ball.speed.y;

          if (score >= bestscore)  bestscore = score;
          // Ballz to da wallz collies
          if ((ball.position.x >= (GetScreenWidth() - ball.radius)) || (ball.position.x <= ball.radius)) {
            ball.speed.x *= -1.0f;
            if (!mute) PlaySoundMulti(fxbounce);
          }

          if ((ball.position.y >= (GetScreenHeight() - ball.radius)) || (ball.position.y <= ball.radius)) {
            ball.speed.y *= -1.0f;
            if (!mute) PlaySoundMulti(fxbounce);
          }

          if (CheckCollisionCircleRec(ball.position, ball.radius, player.hitbox)) {
            player.hp -= GetFrameTime() * 3.0f;
            player.currentframe = 1;
          } else player.currentframe = 0;

          if (ball.radius <= 100)  ball.radius += GetFrameTime() * ball.growth;
        }

     }
     else pauseTimer++;

      break;
    case GAMEOVER:
        if (IsKeyPressed(KEY_UP)) gameoverSelected++;
        if (IsKeyPressed(KEY_DOWN)) gameoverSelected--;
        if (gameoverSelected > 0) gameoverSelected--;
        if (gameoverSelected < -1) gameoverSelected++;

        if ((gameoverSelected == 0) && (IsKeyPressed(KEY_ENTER)))
          currentScreen = GAMEPLAY;

        if ((gameoverSelected == -1) && (IsKeyPressed(KEY_ENTER)))
          currentScreen = TITLE;
        break;
    case CREDITS:
        if (IsKeyPressed(KEY_ENTER)) currentScreen = TITLE;
      default: break;
  }
}

void drawGame(void)
{
  // code to render the game to the game window
  BeginDrawing();

      ClearBackground(RAYWHITE);

      switch(currentScreen) {
        case TITLE:
          DrawRectangle(0, 0, screenWidth, screenHeight, ORANGE);
          DrawText("Controls", 10, 10, 30, PURPLE);
          DrawText(TextFormat("BEST: %i", bestscore), 600, 0, 30, WHITE);
          DrawText("Press the arrow keys to move", 10, 40, 10, RED);
          DrawText("Press 'ENTER' to pause", 10, 60, 10, RED);
          DrawText("Press 'M' to mute", 10, 80, 10, RED);
          DrawText("Press 'Left-ALT' + 'F' for full screen", 10, 100, 10, RED);
          DrawText("Press 'R' to restart", 10, 120, 10, RED);
          DrawText("Press 'ENTER' to select an option", 10, 140, 10, RED);
          DrawText("Avoid", 330, 20, 50, BLUE);
          if (titleSelected == 0) DrawText("PLAY", 360, 220, 20, WHITE);
          else DrawText("PLAY", 360, 220, 20, BLUE);

          if (titleSelected == -1) DrawText("CREDITS", 340, 240, 20, WHITE);
          else DrawText("CREDITS", 340, 240, 20, BLUE);

          if (titleSelected == -2) DrawText("MORE GAMES", 320, 260, 20, WHITE);
          else DrawText("MORE GAMES", 320, 260, 20, BLUE);
          break;

        case GAMEPLAY:
          DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
          DrawFPS(10, 430);
          DrawText(TextFormat("HP: %i", player.hp), 10, 10, 20, RED);
          DrawText(TextFormat("SCORE: %i", score), 10, 30, 20, BLUE);
          DrawText(TextFormat("BALL SIZE: %f", ball.radius), 10, 50, 20, PINK);
          if (ball.active) DrawCircleV(ball.position, (float)ball.radius, ball.color);
          if (ShowHitbox) DrawRectangleRec(player.hitbox, BLUE);
          DrawTextureRec(player.sprite, player.frameRec, player.sprite_pos, WHITE);
          if (pause && ((pauseTimer/30)%2)) DrawText("PAUSED", 330, 190, 30, PURPLE);
          break;

        case GAMEOVER:
          DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
          DrawText("GAMEOVER", 250, 20, 50, RED);
          if (gameoverSelected == 0) DrawText("RETRY", 350, 200, 20, WHITE);
          else DrawText("RETRY", 350, 200, 20, RED);

          if (gameoverSelected == -1) DrawText("TITLE", 352, 230, 20, WHITE);
          else DrawText("TITLE", 352, 230, 20, RED);
          break;

        case CREDITS:
          DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
          DrawText("Avoid", 330, 20, 50, MAGENTA);
          DrawText("Programming and Art by Return0ne", 10, 210, 20, BLUE);
          DrawText("Powered by raylib 4.0", 10, 240, 20, BLUE);
          DrawText("A Canneddonuts project 2022", 10, 270, 40, RED);
          DrawText("Press 'ENTER' ", 10, 350, 20, WHITE);
          break;

        default: break;
      }

  EndDrawing();
}

void gameReset(void)
{
  // code to reset all variables without reloading assets
   player.currentframe = 0;
   player.hp = 30;
   player.hitbox = (Rectangle) {
     GetScreenWidth()/2.0f - 30,
     GetScreenHeight()/2.0f - 30,
     70,
     70
   };

   ball.position = (Vector2){ 50, 50 };
   ball.radius = 20;
   ball.active = true;

   ShowHitbox = 0;

   pauseTimer = 0;
   score = 0;

   gameoverSelected = 0;
}

void gameLoop(void)
{
  updateGame();
  drawGame();
}

void unloadGame(void)
{
  UnloadSound(fxbounce);
  UnloadTexture(player.sprite);
}
