/*
-    Avoid ~ a game by Canneddonuts
-        Filename ~ Main.c
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#include "../include/raylib.h"

#include "Controls.h"

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
    float speed;
    int hp;
    int currentframe;
    Vector2 sprite_pos;
    Rectangle frameRec;
    Rectangle hitbox;
} Player;
/*
typedef struct Item {
    Texture2D sprite;
    Vector2 sprite_pos;
    Rectangle hitbox;
    bool active;
} Item;
*/
// Game variables
static int pauseTimer;
static int score, bestscore;
static int titleSelected = 0, gameoverSelected = 0;
static GameScreen currentScreen = { 0 };
static Sound fxbounce = { 0 };
static Player player = { 0 };
static Ball ball = { 0 };
// static Item heart = { 0 };
static bool pause;
static bool mute;
static bool DebugMode;

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

  CloseAudioDevice();

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
   player.speed = 300.0f;
   player.hp = 30;
   player.frameRec = (Rectangle) {
     player.hitbox.x,
     player.hitbox.y,
    (float) player.sprite.width/3,
    (float) player.sprite.height
   };
   player.hitbox = (Rectangle) {
     GetScreenWidth()/2.0f - 30,
     GetScreenHeight()/2.0f - 30,
     (float) player.sprite.width/3,
     (float) player.sprite.height
   };
/*
  heart.sprite = LoadTexture("assets/gfx/health.png");
  heart.hitbox = (Rectangle) {
    GetRandomValue(0, screenWidth - heart.sprite.width),
    GetRandomValue(0, screenHeight - heart.sprite.height),
    (float) heart.sprite.width/3,
    (float) heart.sprite.height
   };
   heart.active = true;
*/
   ball.position = (Vector2){ 50, 50 };
   ball.speed = (Vector2){ 400.0f,  300.0f };
   ball.radius = 20;
   ball.growth = 2;
   ball.color = MAROON;
   ball.active = true;

   pause = 0;
   mute = 0;
   DebugMode = 0;
   pauseTimer = 0;
   score = 0;
}

void updateGame(void)
{
  // code that runs as long as the program is running
 if ((IsKeyDown(KEY_LEFT_ALT)) && (IsKeyPressed(KEY_F))) ToggleFullscreen();

  switch(currentScreen) {
    case TITLE:
        if (INPUT_UP_PRESSED) titleSelected++;
        if (INPUT_DOWN_PRESSED) titleSelected--;
        if (titleSelected > 0) titleSelected--;
        if (titleSelected < -2) titleSelected++;

        if ((titleSelected == 0) && (INPUT_OPTION_PRESSED)) currentScreen = GAMEPLAY;
        if ((titleSelected == -1) && (INPUT_OPTION_PRESSED)) currentScreen = CREDITS;
        if ((titleSelected == -2) && (INPUT_OPTION_PRESSED)) OpenURL("https://canneddonuts.itch.io/");
     break;
    case GAMEPLAY:

     if (IsKeyPressed(KEY_M)) mute = !mute;

    if (INPUT_OPTION_PRESSED) pause = !pause;

     if (!pause) {
       // Controls
       if (INPUT_LEFT_DOWN) player.hitbox.x -= GetFrameTime() * player.speed;
       if (INPUT_RIGHT_DOWN) player.hitbox.x += GetFrameTime() * player.speed;
       if (INPUT_UP_DOWN) player.hitbox.y -= GetFrameTime() * player.speed;
       if (INPUT_DOWN_DOWN) player.hitbox.y += GetFrameTime() * player.speed;
       if (INPUT_DASH_DOWN) {
         player.speed = 600.0f;
         if (player.currentframe != 1) player.currentframe = 2;
       } else player.speed = 300.0f;


       player.sprite_pos = (Vector2){ player.hitbox.x, player.hitbox.y };
       player.frameRec.x = (float)player.currentframe*(float)player.sprite.width/3;

    //   heart.sprite_pos = (Vector2){ heart.hitbox.x, heart.hitbox.y };

       // Player to da wallz collies
       if ((player.hitbox.x + player.hitbox.width) >= GetScreenWidth()) player.hitbox.x = GetScreenWidth() - player.hitbox.width;
       else if (player.hitbox.x <= 0) player.hitbox.x = 0;

       if ((player.hitbox.y + player.hitbox.height) >= GetScreenHeight()) player.hitbox.y = GetScreenHeight() - player.hitbox.height;
       else if (player.hitbox.y <= 0) player.hitbox.y = 0;

       if (IsKeyPressed(KEY_B)) ball.active = !ball.active;

       if (IsKeyPressed(KEY_D)) DebugMode = !DebugMode;

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
          // movement of the ball
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
        if (INPUT_UP_PRESSED) gameoverSelected++;
        if (INPUT_DOWN_PRESSED) gameoverSelected--;
        if (gameoverSelected > 0) gameoverSelected--;
        if (gameoverSelected < -1) gameoverSelected++;

        if ((gameoverSelected == 0) && (INPUT_OPTION_PRESSED))
          currentScreen = GAMEPLAY;

        if ((gameoverSelected == -1) && (INPUT_OPTION_PRESSED))
          currentScreen = TITLE;
        break;
    case CREDITS:
        if (INPUT_OPTION_PRESSED) currentScreen = TITLE;
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
          DrawText("Press the arrow keys or 'DPAD' to move and 'X' to dash", 10, 40, 10, RED);
          DrawText("Press 'ENTER' or 'START' to pause", 10, 60, 10, RED);
          DrawText("Press 'M' to mute", 10, 80, 10, RED);
          DrawText("Press 'Left-ALT' + 'F' for full screen", 10, 100, 10, RED);
          DrawText("Press 'R' to restart", 10, 120, 10, RED);
          DrawText("Press 'ENTER' or 'START' to select an option", 10, 140, 10, RED);
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
          if (DebugMode) {
            DrawText(TextFormat("BALL SIZE: %f", ball.radius), 10, 50, 20, GREEN);
            DrawText(TextFormat("BALL POS X: %f, BALL POS Y: %f", ball.position.x, ball.position.y), 10, 70, 20, GREEN);
            DrawText(TextFormat("BALL SPEED X: %f, BALL SPEED Y: %f", ball.speed.x, ball.speed.y), 10, 90, 20, GREEN);
            DrawRectangleRec(player.hitbox, BLUE);
          }
          if (ball.active) DrawCircleV(ball.position, (float)ball.radius, ball.color);
        //  if (heart.active) DrawTexture(heart.sprite, heart.sprite_pos.x, heart.sprite_pos.y, RAYWHITE);
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
   player.speed = 300.0f;
   player.hp = 30;
   player.hitbox = (Rectangle) {
     GetScreenWidth()/2.0f - 30,
     GetScreenHeight()/2.0f - 30,
     (float) player.sprite.width/3,
     (float) player.sprite.height
   };
/*
   heart.hitbox = (Rectangle) {
     GetRandomValue(0, screenWidth - heart.sprite.width),
     GetRandomValue(0, screenHeight - heart.sprite.height),
     (float) heart.sprite.width/3,
     (float) heart.sprite.height
    };
    heart.active = true;
*/
   ball.position = (Vector2){ 50, 50 };
   ball.radius = 20;
   ball.active = true;

   DebugMode = 0;

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
  //UnloadTexture(heart.sprite);
}
