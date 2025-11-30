#pragma once

#include "common.h"
#include "utils.h"

struct Item {
  Vector2 pos;

  Item(Vector2 pos) : pos(pos) { TraceLog(LOG_DEBUG, "Created new item!"); }

  void Update(float dt, Vector2 target);
  void Draw(Texture texture);
};