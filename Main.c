#if defined(PLATFORM_WEB)
    #include "/usr/local/include/raylib.h"
    #include <emscripten/emscripten.h>
#else
    #include "raylib.h"
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
    Color color;
    bool active;
} Ball;

typedef struct Player {
    Rectangle hitbox;
    int hp;
} Player;

// Game variables
static GameScreen currentScreen = { 0 };
static Music Bgm01 = { 0 };
static Sound fxbounce = { 0 };
static Player player = { 0 };
static Ball ball = { 0 };
static bool pause;
static bool mute;
static int pauseTimer;
static int BallFrameCounter;
static int selected = 0;

// Game functions
static void GameInit(void);
static void UpdateGame(void);
static void DrawGame(void);
static void UpdateDrawFrame(void);
static void UnloadGame(void);

int main(void)
{
  InitWindow(screenWidth, screenHeight, "Avoid");

  InitAudioDevice();

  GameInit();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
  SetTargetFPS(60);

  while (!WindowShouldClose()) UpdateDrawFrame();
#endif

  UnloadGame();

  CloseWindow();

  return 0;
}

void GameInit(void)
{
   currentScreen = TITLE;

   Bgm01 = LoadMusicStream("assets/bgm/01-Slipin-Sunday.ogg");

   fxbounce = LoadSound("assets/sfx/boing.mp3");


   PlayMusicStream(Bgm01);
   SetMasterVolume(0.2);

   player.hp = 30;
   player.hitbox = (Rectangle) {
     GetScreenWidth()/2.0f - 30,
     GetScreenHeight()/2.0f - 30,
     50,
     50
   };

   ball.position = (Vector2){ 50, 50 };
   ball.radius = 20;
   ball.speed = (Vector2){ 400.0f, 400.0f };
   ball.color = MAROON;
   ball.active = true;

   pause = 0;
   mute = 0;

   pauseTimer = 0;
   BallFrameCounter = 0;
}

void UpdateGame(void)
{
 if ((IsKeyDown(KEY_LEFT_SHIFT)) && (IsKeyPressed(KEY_F))) ToggleFullscreen();

  switch(currentScreen) {
    case TITLE:
        if (IsKeyPressed(KEY_UP)) selected++;
        if (IsKeyPressed(KEY_DOWN)) selected--;
        if (selected > 0) selected--;
        if (selected < -2) selected++;

        if ((selected == 0) && (IsKeyPressed(KEY_ENTER))) currentScreen = GAMEPLAY;
        if ((selected == -1) && (IsKeyPressed(KEY_ENTER))) currentScreen = CREDITS;
        if ((selected == -2) && (IsKeyPressed(KEY_ENTER))) OpenURL("https://gitdab.com/Canneddonuts/Avoid.git");
     break;
    case GAMEPLAY:
     UpdateMusicStream(Bgm01);

     if (IsKeyPressed(KEY_M)) {
       mute = !mute;

       if (mute) PauseMusicStream(Bgm01);
       else ResumeMusicStream(Bgm01);
     }

    if (IsKeyPressed(KEY_ENTER)) pause = !pause;

     if (!pause) {
       // Controls
       if (IsKeyDown(KEY_LEFT)) player.hitbox.x -= GetFrameTime() * 300.0f;
       if (IsKeyDown(KEY_RIGHT)) player.hitbox.x += GetFrameTime() * 300.0f;
       if (IsKeyDown(KEY_UP)) player.hitbox.y -= GetFrameTime() * 300.0f;
       if (IsKeyDown(KEY_DOWN)) player.hitbox.y += GetFrameTime() * 300.0f;

       // Player to da wallz collies
       if ((player.hitbox.x + player.hitbox.width) >= GetScreenWidth()) player.hitbox.x = GetScreenWidth() - player.hitbox.width;
       else if (player.hitbox.x <= 0) player.hitbox.x = 0;

       if ((player.hitbox.y + player.hitbox.height) >= GetScreenHeight()) player.hitbox.y = GetScreenHeight() - player.hitbox.height;
       else if (player.hitbox.y <= 0) player.hitbox.y = 0;

       if (IsKeyPressed(KEY_D)) ball.active = !ball.active;
       if (IsKeyPressed(KEY_R)) { GameInit(); currentScreen = TITLE; }
        if (ball.active) {
          BallFrameCounter++;
          // moveiement oof the balls
          ball.position.x += GetFrameTime() * ball.speed.x;
          ball.position.y += GetFrameTime() * ball.speed.y;


          // Ballz to da wallz collies
          if ((ball.position.x >= (GetScreenWidth() - ball.radius)) || (ball.position.x <= ball.radius)) {
            ball.speed.x *= -1.0f;
            if (!mute) PlaySoundMulti(fxbounce);
          }

          if ((ball.position.y >= (GetScreenHeight() - ball.radius)) || (ball.position.y <= ball.radius)) {
            ball.speed.y *= -1.0f;
            if (!mute) PlaySoundMulti(fxbounce);
          }

          if (CheckCollisionCircleRec(ball.position, ball.radius, player.hitbox)) player.hp--;

          if (BallFrameCounter==1000) ball.radius = 40;
          if (BallFrameCounter==2000) ball.radius = 50;
          if (BallFrameCounter==3000) ball.radius = 60;
          if (BallFrameCounter==4000) ball.radius = 70;
          if (BallFrameCounter==5000) ball.radius = 80;
        }

        if (player.hp <= 0) currentScreen = GAMEOVER;

     }
     else pauseTimer++;

      break;
    case GAMEOVER:
        if (IsKeyPressed(KEY_ENTER)) {
          GameInit();
          currentScreen = GAMEPLAY;
        }
        break;
    case CREDITS:
        if (IsKeyPressed(KEY_ENTER)) currentScreen = TITLE;
      default: break;
  }
}

void DrawGame(void)
{
  BeginDrawing();

      ClearBackground(RAYWHITE);

      switch(currentScreen) {
        case TITLE:
          DrawRectangle(0, 0, screenWidth, screenHeight, ORANGE);
          DrawText("Controls", 10, 10, 30, PURPLE);
          DrawText("Press the arrow keys to move", 10, 40, 10, RED);
          DrawText("Press 'ENTER' to pause", 10, 60, 10, RED);
          DrawText("Press 'M' to mute", 10, 80, 10, RED);
          DrawText("Press 'LSHIFT' + 'F' for full screen", 10, 100, 10, RED);
          DrawText("Press 'R' to restart", 10, 120, 10, RED);
          DrawText("Press 'ENTER' to select an option", 10, 140, 10, RED);
          DrawText("Avoid", 330, 20, 50, BLUE);
          if (selected == 0) DrawText("PLAY", 360, 220, 20, WHITE);
          else DrawText("PLAY", 360, 220, 20, BLUE);

          if (selected == -1) DrawText("CREDITS", 340, 240, 20, WHITE);
          else DrawText("CREDITS", 340, 240, 20, BLUE);

          if (selected == -2) DrawText("SOURCE CODE", 315, 260, 20, WHITE);
          else DrawText("SOURCE CODE", 315, 260, 20, BLUE);
          break;

        case GAMEPLAY:
          DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
          DrawFPS(10, 430);
          DrawText(TextFormat("HP: %i", player.hp), 10, 10, 20, RED);
          DrawText(TextFormat("BALL FRAMES: %i", BallFrameCounter), 10, 30, 20, BLUE);
          DrawText(TextFormat("BALL SIZE: %f", ball.radius), 10, 50, 20, PINK);
          if (ball.active) DrawCircleV(ball.position, (float)ball.radius, ball.color);
          DrawRectangleRec(player.hitbox, BLUE);
          if (pause && ((pauseTimer/30)%2)) DrawText("PAUSED", 330, 190, 30, PURPLE);
          break;

        case GAMEOVER:
          DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
          DrawText("GAMEOVER", 250, 20, 50, RED);
          DrawText("PRESS ENTER TO RESET", 270, 220, 20, WHITE);
          break;

        case CREDITS:
          DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
          DrawText("Avoid", 330, 20, 50, PINK);
          DrawText("Programming by M-C-O-B", 10, 210, 20, BLUE);
          DrawText("Morale support by Tobi/Tobrella and Jelly_man", 10, 240, 20, BLUE);
          DrawText("Powered by raylib 4.0", 10, 270, 20, BLUE);
          DrawText("A Canneddonuts project 2022", 10, 310, 40, RED);
          DrawText("Press 'ENTER' ", 10, 350, 20, WHITE);
          break;

        default: break;
      }

  EndDrawing();
}


void UpdateDrawFrame(void)
{
  UpdateGame();
  DrawGame();
}

void UnloadGame(void)
{
  UnloadMusicStream(Bgm01);
  UnloadSound(fxbounce);
}
