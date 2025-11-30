#include "game_over.h"

void GameOver::Load() {}

void GameOver::Update(float dt) {}

void GameOver::Draw()
{
  DrawText("You Died.", (MeasureText("You Died.", 48) / 2) - (SCREEN_WIDTH / 2), 24 - (SCREEN_HEIGHT / 2), 48, RED);

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

void GameOver::Unload() {}