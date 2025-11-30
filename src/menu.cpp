#include "menu.h"

void Menu::Load()
{
}

void Menu::Update(float dt)
{
  UpdateMusicStream(bg_music);
}

void Menu::Draw()
{
  DrawText("Escape Plan", 8, 8, 50, WHITE);
  DrawLineEx(Vector2{0, 54}, Vector2{600, 54}, 2, WHITE);
  DrawText("You've crashed landed on an alien planet.\nGather carbon by breaking rocks and deposit it in the drop pod.\n\nCraft tethers to travel further from the drop pod.\nFollow the flag icon to get to the pickup point.\n\nWASD to move\n\nLEFT CLICK to shoot\n\nRIGHT CLICK to break rocks\n\nSPACE to place tethers",
           8, 64,
           20,
           WHITE);
  if (GuiButton(Rectangle{0, SCREEN_HEIGHT - 48, 128, 48}, "Start Game"))
  {
    ChangeState(GameState::GAME);
  }
}

void Menu::Unload() {}