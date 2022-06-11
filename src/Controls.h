/*
-    Avoid ~ a game by Canneddonuts
-      Filename ~ Controls.h
-       Author ~ Return0ne
-            2022
-         *no license*
*/

#ifndef CONTROLS_HEADER
#define CONTROLS_HEADER

#define INPUT_UP_PRESSED IsKeyPressed(KEY_UP) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP)
#define INPUT_DOWN_PRESSED IsKeyPressed(KEY_DOWN) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)

#define INPUT_OPTION_PRESSED IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)

#define INPUT_LEFT_DOWN IsKeyDown(KEY_LEFT) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)
#define INPUT_RIGHT_DOWN IsKeyDown(KEY_RIGHT) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)
#define INPUT_UP_DOWN IsKeyDown(KEY_UP) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP)
#define INPUT_DOWN_DOWN IsKeyDown(KEY_DOWN) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)

#define INPUT_DASH_DOWN IsKeyDown(KEY_X) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)

#endif
