#pragma once

#include "common.h"
#include "raygui.h"
#include "utils.h"

enum ItemTypes { TETHER, PATCH, AMMO };

struct Droppod {
 public:
  Vector2 pos;
  Rectangle bounds = {0, 0, 24, 8};

  int carbon = 0;
  int days_left = 15;

  bool showInfoPanel = false;
  bool showCraftingPanel = false;

  Vector2 GetCenter() {
    return Vector2{pos.x + (bounds.width / 2), pos.y + (bounds.height / 2)};
  }

  void CraftItem();

  void Draw(Texture spritesheet);
};