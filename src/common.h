#pragma once

#include <raylib.h>
#include <raymath.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;
const int GAME_WIDTH = 240;
const int GAME_HEIGHT = 126;
const int WORLD_WIDTH = 500;
const int WORLD_HEIGHT = 500;
const int SPRITESHEET_WIDTH = 8;
const int SPRITESHEET_HEIGHT = 8;
const int CELL_SIZE = 8;

const Color TETHER_COLOR = Color{182, 207, 142, 255};

enum GameState
{
  MENU,
  GAME,
  GAME_OVER,
  GAME_WON
};

extern Camera2D *game_camera;
extern Sound squib_hurt;
extern Sound squib_die;
extern Sound o2_alarm;
extern Sound shoot;
extern Sound hurt;
extern Sound craft;
extern Sound break_rock;
extern Sound deposit;
extern Music bg_music;

void SetPlayerCanShoot(bool can_shoot);
void ChangeState(enum GameState new_state);
void SpawnBullet(Vector2 position, Vector2 dir, float angle);