#pragma once

#include "common.h"
#include "raygui.h"
#include "utils.h"

struct GameOver
{
  void Load();
  void Update(float dt);
  void Draw();
  void Unload();
};