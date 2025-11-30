#pragma once

#include <map>
#include <vector>

#include "common.h"
#include "item.h"
#include "utils.h"

struct Tile {
  int index;
  bool isSolid;
  bool isRock;
  int health;

  Tile(int index = 3, bool isSolid = false, int health = 0, bool isRock = false)
      : index(index), isSolid(isSolid), health(health), isRock(isRock) {}
};

struct World {
  std::vector<Tile> tiles;
  std::vector<Item> items;

  World();
  ~World();

  Tile* GetTile(int x, int y);
  void SetTile(int x, int y, Tile tile);
  void SetTile(int idx, Tile tile);

  void DamageTile(int x, int y, int amount);

  Vector2i GetCenter() { return Vector2i{WORLD_WIDTH / 2, WORLD_HEIGHT / 2}; }
  bool IsSolid(int x, int y);

  void DrawMap(Texture2D spritesheet, Rectangle region);
};