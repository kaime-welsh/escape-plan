#pragma once

#include <raylib.h>

#include "utils.h"
#include "world.h"

struct Player
{
  Vector2 pos;
  Vector2 vel;
  float speed = 25;
  int hp = 10;
  int max_hp = 10;
  float timer = 10;

  int carbon = 0;
  int max_carbon = 20;

  int tethers = 0;
  bool connected_to_tether = false;

  int break_dist = 14;
  float break_timer;
  float break_time = 0.2;

  int selected_item = 0;

  void Update(World *world, float dt);
  void Draw(Texture2D spritesheet);
};