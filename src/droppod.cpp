#include "droppod.h"

void Droppod::CraftItem() {}

void Droppod::Draw(Texture spritesheet) {
  DrawSprite(spritesheet, 9, pos.x, pos.y, 1.0);
  DrawSprite(spritesheet, 10, pos.x + 8, pos.y, 1.0);
  DrawSprite(spritesheet, 11, pos.x + 16, pos.y, 1.0);
}