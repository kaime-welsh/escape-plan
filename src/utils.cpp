#include "utils.h"

int CoordToIndex(int x, int y) { return y * WORLD_WIDTH + x; }

Vector2 IndexToVector2(int index) {
  return Vector2{float(index % WORLD_WIDTH), float(index / WORLD_HEIGHT)};
}
Vector2i IndexToVector2i(int index) {
  return Vector2i{index % WORLD_WIDTH, index / WORLD_HEIGHT};
}

void DrawSprite(Texture2D spritesheet, int index, Vector2 pos, float scale,
                Color tint) {
  Rectangle src = Rectangle{float(index % SPRITESHEET_WIDTH) * CELL_SIZE,
                            float(index / SPRITESHEET_WIDTH) * CELL_SIZE,
                            float(CELL_SIZE), float(CELL_SIZE)};
  Rectangle dest =
      Rectangle{pos.x, pos.y, CELL_SIZE * scale, CELL_SIZE * scale};
  DrawTexturePro(spritesheet, src, dest, Vector2{0, 0}, 0.0f, tint);
}

void DrawSprite(Texture2D spritesheet, int index, Vector2i pos, float scale,
                Color tint) {
  DrawSprite(spritesheet, index, Vector2{float(pos.x), float(pos.y)}, scale,
             tint);
}

void DrawSprite(Texture2D spritesheet, int index, int x, int y, float scale,
                Color tint) {
  DrawSprite(spritesheet, index, Vector2{float(x), float(y)}, scale, tint);
}