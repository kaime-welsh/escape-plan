#pragma once

#include <raylib.h>

#include "utils.h"
#include "world.h"

struct Player
{
  Vector2 pos;
  Vector2 vel;
  float speed = 25;
  int hp = 5;
  int max_hp = 5;
  float timer = 5;
  float timer_max = 5;

  int carbon = 0;
  int max_carbon = 50;

  int tethers = 0;
  bool connected_to_tether = false;

  int break_dist = 14;
  float break_timer;
  float break_time = 0.2;

  bool can_shoot = false;
  int ammo = 32;
  int max_ammo = 256;

  float fire_timer;
  float fire_time = 0.25;

  float damage_timer;
  float damage_time = 1.0;

  bool near_pickup = false;

  void Update(World *world, float dt);
  void Draw(Texture2D spritesheet);
  void Damage(int amount);
};