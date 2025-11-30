#include "player.h"

void Player::Update(World *world, float dt)
{
  if (break_timer > 0)
  {
    break_timer -= dt;
  }

  if (connected_to_tether == false && timer > 0)
  {
    timer -= dt;
  }
  else if (connected_to_tether == false && timer <= 0)
  {
    Damage(1);
    timer = timer_max;
  }
  else if (connected_to_tether)
  {
    timer += 2 * dt;
  }
  timer = Clamp(timer, 0, timer_max);

  if (damage_timer > 0)
  {
    damage_timer -= dt;
  }

  if (IsKeyDown(KEY_W) && !world->IsSolid(pos.x, pos.y - 1) &&
      !world->IsSolid(pos.x + 7, pos.y - 1))
    pos.y -= 1;
  else if (IsKeyDown(KEY_S) && !world->IsSolid(pos.x, pos.y + 8) &&
           !world->IsSolid(pos.x + 7, pos.y + 8))
    pos.y += 1;
  if (IsKeyDown(KEY_A) && !world->IsSolid(pos.x - 1, pos.y) &&
      !world->IsSolid(pos.x - 1, pos.y + 7))
    pos.x -= 1;
  else if (IsKeyDown(KEY_D) && !world->IsSolid(pos.x + 8, pos.y) &&
           !world->IsSolid(pos.x + 8, pos.y + 7))
    pos.x += 1;

  hp = Clamp(hp, 0, max_hp);

  // Shoot bullet
  float angle = atan2(GetMouseY() - (SCREEN_HEIGHT / 2), GetMouseX() - (SCREEN_WIDTH / 2)) * (180.0f / PI);
  fire_timer -= dt;
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && can_shoot)
  {
    if (fire_timer <= 0 && ammo > 0)
    {
      Vector2 dir = Vector2Normalize(Vector2Subtract(GetScreenToWorld2D(GetMousePosition(), (*game_camera)), pos));
      float a = atan2(dir.y, dir.x);
      SpawnBullet(Vector2{pos.x + 4, pos.y + 4}, dir, a - 0.1);
      SpawnBullet(Vector2{pos.x + 4, pos.y + 4}, dir, a);
      SpawnBullet(Vector2{pos.x + 4, pos.y + 4}, dir, a + 0.1);
      ammo -= 1;
      fire_timer = fire_time;
      PlaySound(shoot);
    }
  }
  // hp = max_hp;
}

void Player::Draw(Texture2D spritesheet)
{
  DrawSprite(spritesheet, 0, pos, 1.0);
}

void Player::Damage(int amount)
{
  if (damage_timer <= 0)
  {
    hp -= 1;
    damage_timer = damage_time;
    PlaySound(hurt);
  }
}