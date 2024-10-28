#include "utils.h"

int CoordToIndex(int x, int y) { return y * WORLD_WIDTH + x; }

Vector2 IndexToVector2(int index) {
  return Vector2{float(index % WORLD_WIDTH), float(index / WORLD_HEIGHT)};
}
Vector2i IndexToVector2i(int index) {
  return Vector2i{index % WORLD_WIDTH, index / WORLD_HEIGHT};
}

void DrawSprite(int index, Vector2 pos) {
  Rectangle src = Rectangle{float(index % SPRITESHEET_WIDTH) * CELL_SIZE,
                            float(index / SPRITESHEET_WIDTH) * CELL_SIZE,
                            float(CELL_SIZE), float(CELL_SIZE)};
  Rectangle dest = Rectangle{pos.x, pos.y, CELL_SIZE, CELL_SIZE};
  DrawTexturePro(spritesheet, src, dest, Vector2{0, 0}, 0.0f, WHITE);
}

void DrawSprite(int index, Vector2i pos) {
  DrawSprite(index, Vector2{float(pos.x), float(pos.y)});
}

void DrawSprite(int index, int x, int y) {
  DrawSprite(index, Vector2{float(x), float(y)});
}