#include "item.h"

void Item::Update(float dt, Vector2 target) {
  if (Vector2Distance(pos, target) > 24) return;
  pos.x += (target.x - pos.x) * 8 * dt;
  pos.y += (target.y - pos.y) * 8 * dt;
}

void Item::Draw(Texture spritesheet) {
  DrawSprite(spritesheet, 8, pos.x, pos.y, 1.0f);
}