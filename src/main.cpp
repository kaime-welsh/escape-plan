#include "common.h"
#include "game.h"
#include "game_over.h"
#include "game_won.h"
#include "menu.h"
#include "player.h"
#include <vector>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

enum GameState state = GameState::MENU;
Menu menu;
Game game;
GameOver game_over;
GameWon game_won;
Camera2D *game_camera;

Sound squib_hurt;
Sound squib_die;
Sound o2_alarm;
Sound shoot;
Sound hurt;
Sound craft;
Sound break_rock;
Sound deposit;
Music bg_music;

void SetPlayerCanShoot(bool can_shoot)
{
  game.player.can_shoot = can_shoot;
}

void SpawnBullet(Vector2 pos, Vector2 dir, float angle)
{
  game.SpawnBullet(pos, dir, angle);
}

void ChangeState(enum GameState new_state)
{
  switch (state)
  {
  case MENU:
    menu.Unload();
    break;
  case GAME:
    game.Unload();
    break;
  case GAME_OVER:
    game_over.Unload();
    break;
  case GAME_WON:
    game_won.Unload();
    break;
  }

  state = new_state;
  switch (state)
  {
  case MENU:
    menu = Menu();
    menu.Load();
    break;
  case GAME:
    game = Game();
    game.Load();
    break;
  case GAME_OVER:
    game_over = GameOver();
    game_over.Load();
    break;
  case GAME_WON:
    game_won = GameWon();
    game_won.Load();
    break;
  }
}

static void UpdateDrawFrame()
{
  float dt = GetFrameTime();
  switch (state)
  {
  case MENU:
    menu.Update(dt);
    break;
  case GAME:
    game.Update(dt);
    break;
  case GAME_OVER:
    game_over.Update(dt);
    break;
  case GAME_WON:
    game_won.Update(dt);
    break;
  }

  BeginDrawing();
  ClearBackground(Color{15, 15, 15, 255});
  switch (state)
  {
  case MENU:
    menu.Draw();
    break;
  case GAME:
    game.Draw();
    break;
  case GAME_OVER:
    game_over.Draw();
    break;
  case GAME_WON:
    game_won.Draw();
    break;
  }
  EndDrawing();
}

int main(void)
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Escape Plan");
  GuiLoadStyle("resources/dark/style_dark.rgs");
  InitAudioDevice();
  SetTraceLogLevel(LOG_ALL);

  squib_hurt = LoadSound("resources/audio/squib.ogg");
  SetSoundVolume(squib_hurt, 0.1);
  squib_die = LoadSound("resources/audio/squib_die.ogg");
  SetSoundVolume(squib_die, 0.1);
  shoot = LoadSound("resources/audio/shoot.ogg");
  SetSoundVolume(shoot, 0.2);
  o2_alarm = LoadSound("resources/audio/o2_alarm.ogg");
  SetSoundVolume(o2_alarm, 0.4);
  hurt = LoadSound("resources/audio/hurt.ogg");
  SetSoundVolume(hurt, 0.4);
  craft = LoadSound("resources/audio/craft.ogg");
  SetSoundVolume(craft, 0.2);
  break_rock = LoadSound("resources/audio/break_rock.ogg");
  SetSoundVolume(break_rock, 0.2);
  deposit = LoadSound("resources/audio/deposit.ogg");
  SetSoundVolume(deposit, 0.2);

  bg_music = LoadMusicStream("resources/audio/bg_music.ogg");
  SetMusicVolume(bg_music, 0.1);
  PlayMusicStream(bg_music);

  switch (state)
  {
  case MENU:
    menu.Load();
    break;
  case GAME:
    game.Load();
    break;
  case GAME_OVER:
    game_over.Load();
    break;
  case GAME_WON:
    game_won.Load();
    break;
  }

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
  SetTargetFPS(60);

  while (!WindowShouldClose())
  {
    UpdateDrawFrame();
  }
#endif
  switch (state)
  {
  case MENU:
    menu.Unload();
    break;
  case GAME:
    game.Unload();
    break;
  case GAME_OVER:
    game_over.Unload();
    break;
  case GAME_WON:
    game_won.Unload();
    break;
  }
  UnloadSound(squib_hurt);
  UnloadSound(squib_die);
  UnloadSound(shoot);
  UnloadSound(hurt);
  UnloadSound(o2_alarm);
  CloseWindow();
  return 0;
}