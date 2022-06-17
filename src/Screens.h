/*
-    Avoid ~ a game by Canneddonuts
-      Filename ~ Screens.h
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#ifndef SCREENS_HEADER
#define SCREENS_HEADER

typedef enum GameScreen { TITLE = 0, GAMEPLAY, GAMEOVER, CREDITS } GameScreen;

extern GameScreen currentScreen;

void gameReset(void);

void InitTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);
void UnloadTitleScreen(void);


void InitGameplayScreen(void);
void UpdateGameplayScreen(void);
void DrawGameplayScreen(void);
void UnloadGameplayScreen(void);
void ResetGameplayScreen(void);

void UpdateCreditsScreen(void);
void DrawCreditsScreen(void);

void InitGameoverScreen(void);
void UpdateGameoverScreen(void);
void DrawGameoverScreen(void);


#endif
