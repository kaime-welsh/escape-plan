#pragma once

#include "common.h"
#include "droppod.h"
#include "item.h"
#include "player.h"
#include "squib.h"
#include "tether.h"
#include "texture_button.h"
#include "utils.h"
#include "world.h"

struct Bullet
{
  Vector2 pos;
  Vector2 dir;
  float angle;
  float lifetime = 1.0;
  bool alive = true;
};

struct Game
{
  Camera2D camera;
  Camera2D ui_camera;
  Texture2D spritesheet;
  World world;
  Player player;
  Droppod pod;
  Vector2 mouse_pos;
  std::vector<Bullet> bullets;
  std::vector<Tether> tethers;
  std::vector<Squib> squibs;
  Vector2 pickup_pos;
  bool paused = false;
  bool draw_tutorial = true;

  bool start_pickup = false;
  float pickup_timer = 120;

  int max_squibs = 10;

  float squib_spawn_timer;
  float squib_spawn_time = 45;
  Vector2 squib_spawn_range = Vector2{100, 500};
  Vector2 squib_spawn_amount = Vector2{5, 20};

  void SpawnBullet(Vector2 pos, Vector2 dir, float angle);

  void Load();
  void Update(float dt);
  void Draw();
  void Unload();
};