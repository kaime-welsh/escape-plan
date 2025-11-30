#pragma once

#include "common.h"
#include "raygui.h"
#include "utils.h"

struct GameWon
{
  void Load();
  void Update(float dt);
  void Draw();
  void Unload();
};