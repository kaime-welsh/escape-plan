#pragma once

#include "common.h"

struct Vector2i {
  int x;
  int y;
};

int CoordToIndex(int x, int y);
Vector2 IndexToVector2(int index);
Vector2i IndexToVector2i(int index);
void DrawSprite(Texture2D spritesheet, int index, Vector2 pos, float scale,
                Color tint = WHITE);
void DrawSprite(Texture2D spritesheet, int index, Vector2i pos, float scale,
                Color tint = WHITE);
void DrawSprite(Texture2D spritesheet, int index, int x, int y, float scale,
                Color tint = WHITE);