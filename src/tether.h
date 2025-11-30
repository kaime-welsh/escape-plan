#pragma once

#include "common.h"
#include "utils.h"

struct Tether
{
  Vector2 pos;
  float range = 64;
  bool is_connected = false;
};