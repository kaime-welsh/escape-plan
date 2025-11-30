#include "texture_button.h"

bool TextureButton(Texture spritesheet, int index, Rectangle bounds,
                   const char *text)
{
  unsigned char alpha = 255;
  if (!CheckCollisionPointRec(GetMousePosition(), bounds))
  {
    alpha = 80;
  }

  DrawSprite(spritesheet, index, bounds.x, bounds.y, 4.0,
             Color{255, 255, 255, alpha});
  DrawRectangleLinesEx(bounds, 2.0f, LIGHTGRAY);
  DrawText(text, bounds.x + 34, bounds.y + 4, 8, Color{255, 255, 255, alpha});

  if (CheckCollisionPointRec(GetMousePosition(), bounds))
  {

    DrawRectangleRec(bounds, Color{84, 84, 84, 255});
    DrawSprite(spritesheet, index, bounds.x, bounds.y, 4.0,
               Color{255, 255, 255, alpha});
    DrawRectangleLinesEx(bounds, 2.0f, LIGHTGRAY);
    DrawText(text, bounds.x + 34, bounds.y + 4, 8, Color{255, 255, 255, alpha});
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
      return true;
    }
  }
  return false;
}