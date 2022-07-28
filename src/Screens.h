/*
-    Avoid ~ a game by Canneddonuts
-      Filename ~ Screens.h
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#ifndef SCREENS_HEADER
#define SCREENS_HEADER

typedef enum GameScreen { TITLE = 0, GAMEPLAY, GAMEOVER, CREDITS, OPTIONS, ENDING } GameScreen;

extern GameScreen currentScreen;

void InitTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);
void UnloadTitleScreen(void);
int FinishTitleScreen(void);

void InitGameplayScreen(void);
void UpdateGameplayScreen(void);
void DrawGameplayScreen(void);
void UnloadGameplayScreen(void);
void LoadGamplayScreen(void);
int FinishGameplayScreen(void);

void InitCreditsScreen(void);
void UpdateCreditsScreen(void);
void DrawCreditsScreen(void);
void UnloadCreditsScreen(void);
int FinishCreditsScreen(void);

void InitGameoverScreen(void);
void UpdateGameoverScreen(void);
void DrawGameoverScreen(void);
void UnloadGameoverScreen(void);
int FinishGameoverScreen(void);

void InitOptionsScreen(void);
void UpdateOptionsScreen(void);
void DrawOptionsScreen(void);
void UnloadOptionsScreen(void);
int FinishOptionsScreen(void);

void InitEndingScreen(void);
void UpdateEndingScreen(void);
void DrawEndingScreen(void);
void UnloadEndingScreen(void);
int FinishEndingScreen(void);

#endif
