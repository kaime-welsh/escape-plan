#include "game_won.h"

void GameWon::Load() {}

void GameWon::Update(float dt) {}

void GameWon::Draw()
{
  DrawText("You Escaped!", (MeasureText("You Escaped!", 48) / 2) - (SCREEN_WIDTH / 2), 24 - (SCREEN_HEIGHT / 2), 48, RED);

  if (GuiButton(Rectangle{
                    SCREEN_WIDTH / 2 - 64,
                    SCREEN_HEIGHT / 2 - 52,
                    128,
                    24},
                "Try Again?"))
  {
    ChangeState(GameState::GAME);
  }
  if (GuiButton(Rectangle{
                    SCREEN_WIDTH / 2 - 64,
                    SCREEN_HEIGHT / 2 - 24,
                    128,
                    24},
                "Quit"))
  {
    ChangeState(GameState::MENU);
  }
}

void GameWon::Unload() {}
