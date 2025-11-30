#include "world.h"

World::World()
{
  for (int idx = 0; idx < WORLD_WIDTH * WORLD_HEIGHT; idx++)
  {
    tiles.push_back(Tile(5, false, 0, false));
    // Create random rocks
    if (GetRandomValue(0, 100) <= 10)
    {
      tiles[idx] = Tile(3, true, 3, true);
    }
    Vector2i coords = IndexToVector2i(idx);
    if (coords.x == 0 || coords.x == WORLD_WIDTH - 1 || coords.y == 0 ||
        coords.y == WORLD_HEIGHT - 1)
    {
      tiles[idx] = Tile(4, true, 0, false);
    }
  }
  // Clear center tile for player
  SetTile(WORLD_WIDTH / 2, WORLD_HEIGHT / 2, Tile(5, false, 0, false));
  // Make tiles near pod solid
  SetTile((WORLD_WIDTH / 2) - 1, WORLD_HEIGHT / 2, Tile(5, true, 0, false));
  SetTile((WORLD_WIDTH / 2) - 2, WORLD_HEIGHT / 2, Tile(5, true, 0, false));
  SetTile((WORLD_WIDTH / 2) - 3, WORLD_HEIGHT / 2, Tile(5, true, 0, false));
}
World::~World() {}

Tile *World::GetTile(int x, int y) { return &tiles[CoordToIndex(x, y)]; }

void World::SetTile(int x, int y, Tile tile)
{
  tiles[CoordToIndex(x, y)] = tile;
}

void World::SetTile(int idx, Tile tile) { tiles[idx] = tile; }

void World::DamageTile(int x, int y, int amount)
{
  Tile *tile = GetTile(x, y);
  if (tile->isRock)
  {
    tile->health -= amount;
    tile->index -= 1;
  }

  if (tile->health <= 0 && tile->isRock)
  {
    tiles[CoordToIndex(x, y)] = Tile(5, false, 0, false);
    items.push_back(Item(Vector2{float(x) * CELL_SIZE, float(y) * CELL_SIZE}));
  }
}

bool World::IsSolid(int x, int y)
{
  x = int(x / CELL_SIZE);
  y = int(y / CELL_SIZE);
  if (x <= 0 || x >= WORLD_WIDTH || y <= 0 || y >= WORLD_HEIGHT)
    return true;
  if (tiles[CoordToIndex(x, y)].isSolid)
    return true;
  return false;
}

void World::DrawMap(Texture2D spritesheet, Rectangle region)
{
  for (int idx = 0; idx < WORLD_WIDTH * WORLD_HEIGHT; idx++)
  {
    Vector2 pos = {
        float(IndexToVector2(idx).x) * CELL_SIZE,
        float(IndexToVector2(idx).y) * CELL_SIZE,
    };
    // Need to do this better, rather than checking EVERY world tile, should
    // just check only the tiles in the region
    if (CheckCollisionRecs(Rectangle{pos.x, pos.y, CELL_SIZE, CELL_SIZE},
                           region))
    {
      DrawSprite(spritesheet, tiles[idx].index, pos, 1.0f);
    }
  }
  for (auto item : items)
  {
    item.Draw(spritesheet);
  }
}