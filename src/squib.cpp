#include "squib.h"

void Squib::Draw(Texture spritesheet)
{
    DrawTexturePro(
        spritesheet,
        Rectangle{
            40, 16,
            16, 16},
        Rectangle{
            pos.x + 8, pos.y + 8,
            16, 16},
        Vector2{8, 8},
        0.0f,
        WHITE);
}
