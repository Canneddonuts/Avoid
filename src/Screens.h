/*
-    Avoid ~ a game by Canneddonuts
-      Filename ~ Screens.h
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#ifndef SCREENS_HEADER
#define SCREENS_HEADER

typedef enum GameScreen { TITLE = 0, GAMEPLAY, GAMEOVER, CREDITS, OPTIONS, ENDING, LEVELSEL } GameScreen;
typedef enum Levels { LEVEL1 = 0, LEVEL2, LEVEL3 } Levels;

extern GameScreen currentScreen;
extern Levels level;
extern int nextlevel;
extern bool levelunlocked[3];


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

void InitLevelSelScreen(void);
void UpdateLevelSelScreen(void);
void DrawLevelSelScreen(void);
void UnloadLevelSelScreen(void);
int FinishLevelSelScreen(void);

#endif
