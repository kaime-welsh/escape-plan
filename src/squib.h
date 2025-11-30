#pragma once

#include "common.h"
#include "utils.h"

struct Squib
{
  Vector2 pos;
  int health = 3;
  int damage = 1;
  float timer = 0.0f;

  bool alive = true;

  Squib(Vector2 pos) : pos(pos) {}
  ~Squib() {}

  // void Update(float dt);
  void Draw(Texture spritesheet);
};