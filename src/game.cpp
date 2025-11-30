#include "game.h"

void Game::SpawnBullet(Vector2 pos, Vector2 dir, float angle)
{
  bullets.push_back(Bullet{pos, dir, angle});
  TraceLog(LOG_DEBUG, TextFormat("Added bullet # %d", bullets.size()));
}

void Game::Load()
{
  // PlayMusicStream(bg_music);
  camera = Camera2D{};
  camera.offset = Vector2{GAME_WIDTH / 2, GAME_HEIGHT / 2};
  game_camera = &camera;

  ui_camera = Camera2D{};

  spritesheet = LoadTexture("resources/spritesheet.png");
  SetTextureFilter(spritesheet, TEXTURE_FILTER_POINT);
  world = World();

  player = Player();
  //* Place player in the center of the world
  player.pos = Vector2{float(world.GetCenter().x) * CELL_SIZE,
                       float(world.GetCenter().y * CELL_SIZE)};
  pod.pos = Vector2{player.pos.x - 24, player.pos.y};
  pod.bounds.x = pod.pos.x;
  pod.bounds.y = pod.pos.y;

  tethers.push_back(Tether(Vector2{pod.pos.x + 12, pod.pos.y + 4}));
  tethers.back().is_connected = true;

  // Add Shuttle flag
  Vector2 end_pos = Vector2{
      float(GetRandomValue(1, WORLD_WIDTH - 2)),
      float(GetRandomValue(1, WORLD_HEIGHT - 2))};

  if (world.GetTile(end_pos.x, end_pos.y)->isRock)
  {
    world.SetTile(end_pos.x, end_pos.y, Tile(5, false, 0, false));
  }
  pickup_pos = Vector2{
      float(int(end_pos.x) * CELL_SIZE),
      float(int(end_pos.y) * CELL_SIZE),
  };
  TraceLog(LOG_DEBUG, TextFormat("Pickup Pos - x: %i y: %i", int(pickup_pos.x / CELL_SIZE), int(pickup_pos.y / CELL_SIZE)));
}

void Game::Update(float dt)
{
  UpdateMusicStream(bg_music);
  if (IsKeyPressed(KEY_ESCAPE))
    paused = !paused;
  if (!paused)
  {
    //* Maintain vertical size
    camera.offset = Vector2{(SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2)};
    camera.zoom = (float(SCREEN_HEIGHT) / float(GAME_HEIGHT));
    ui_camera.zoom = float(SCREEN_HEIGHT) / float(GAME_HEIGHT);
    player.Update(&world, dt);
    player.can_shoot = true;

    // Shuttle countdown timer
    if (start_pickup == true)
    {
      pickup_timer -= dt;
    }

    //* Spawn Squibs
    if (squib_spawn_timer > 0)
    {
      squib_spawn_timer -= dt;
    }
    else
    {
      squib_spawn_timer = squib_spawn_time;
      int spawn_amount = GetRandomValue(squib_spawn_amount.x, squib_spawn_amount.y);
      int spawn_range = GetRandomValue(squib_spawn_range.x, squib_spawn_range.y);

      if (squibs.size() + spawn_amount > max_squibs)
      {
        spawn_amount = squibs.size() - spawn_amount;
      }

      for (int i = 0; i < spawn_amount; i++)
      {
        Vector2 pos = Vector2{
            float(GetRandomValue(-(WORLD_WIDTH * CELL_SIZE), (WORLD_WIDTH * 2) * CELL_SIZE)), float(GetRandomValue(-(WORLD_HEIGHT * CELL_SIZE), (WORLD_HEIGHT * 2) * CELL_SIZE))};
        while (Vector2Distance(player.pos, pos) <= spawn_range)
        {
          pos = Vector2{
              float(GetRandomValue(0, WORLD_WIDTH * CELL_SIZE)), float(GetRandomValue(0, WORLD_HEIGHT * CELL_SIZE))};
        }

        squibs.push_back(Squib(pos));
      }
    }

    // Check item collision with player
    for (auto it = world.items.begin(); it != world.items.end();)
    {
      if (player.carbon < player.max_carbon)
      {
        (*it).Update(dt, player.pos);
      }
      else
      {
        break;
      }

      if (CheckCollisionCircles((*it).pos, 2, player.pos, 2))
      {
        it = world.items.erase(it);
        player.carbon++;
      }
      else
      {
        ++it;
      }
    }

    // Show crafting panel
    if (CheckCollisionRecs(
            Rectangle{player.pos.x, player.pos.y, CELL_SIZE, CELL_SIZE},
            Rectangle{pod.pos.x - 4, pod.pos.y - 4, 36, 22}))
    {
      if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        pod.showInfoPanel = true;
    }
    else
    {
      pod.showInfoPanel = false;
    }

    camera.target = Vector2Add(player.pos, Vector2(4, 4));

    // Do damage to clicked rocks
    mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera);
    Vector2 mouse_world_pos = Vector2{float(int(mouse_pos.x / CELL_SIZE)),
                                      float(int(mouse_pos.y / CELL_SIZE))};
    if (world.GetTile(mouse_world_pos.x, mouse_world_pos.y)->isRock &&
        IsMouseButtonDown(MOUSE_BUTTON_RIGHT) &&
        Vector2Distance(Vector2{mouse_world_pos.x * CELL_SIZE,
                                mouse_world_pos.y * CELL_SIZE},
                        player.pos) <= player.break_dist)
    {
      if (player.break_timer <= 0)
      {
        world.DamageTile(mouse_world_pos.x, mouse_world_pos.y, 1);
        player.break_timer = player.break_time;
        PlaySound(break_rock);
      }
    }

    // Place tether if near another tether
    if (player.tethers > 0 && IsKeyPressed(KEY_SPACE))
    {
      bool can_place = false;
      for (auto tether : tethers)
      {
        if (CheckCollisionPointCircle(
                Vector2{float(player.pos.x) + 4, float(player.pos.y) + 4},
                tether.pos, tether.range))
        {
          can_place = true;
        }
      }

      if (can_place == true)
      {
        tethers.push_back(Tether(Vector2{player.pos.x + 4, player.pos.y + 4}));
        player.tethers -= 1;
      }
    }

    // Check if player is connected to tether
    player.connected_to_tether = false;
    for (auto tether : tethers)
    {
      if (CheckCollisionPointCircle(Vector2{player.pos.x + 4, player.pos.y + 4},
                                    Vector2{tether.pos.x, tether.pos.y}, tether.range))
      {
        player.connected_to_tether = true;
      }
    }

    if (player.hp <= 0)
    {
      ChangeState(GameState::GAME_OVER);
    }

    // Update squibs
    for (auto squib = squibs.begin(); squib != squibs.end();)
    {
      Vector2 dir = Vector2Normalize(Vector2Subtract(player.pos, (*squib).pos));
      (*squib).pos.x += dir.x * 15 * dt;
      (*squib).pos.y += dir.y * 15 * dt;
      if (CheckCollisionCircles(Vector2{player.pos.x + 4, player.pos.y + 4}, 3, Vector2{(*squib).pos.x + 8, (*squib).pos.y + 8}, 4))
      {
        player.Damage(1);
      }

      if ((*squib).health <= 0)
        (*squib).alive = false;

      if (!(*squib).alive)
      {
        PlaySound(squib_die);
        squib = squibs.erase(squib);
      }
      else
      {
        squib++;
      }
    }

    // Update bullets
    for (auto bullet = bullets.begin(); bullet != bullets.end();)
    {
      (*bullet).lifetime -= dt;
      (*bullet).pos.x += (cos((*bullet).angle)) * 125 * dt;
      (*bullet).pos.y += (sin((*bullet).angle)) * 125 * dt;

      if ((*bullet).lifetime <= 0)
      {
        (*bullet).alive = false;
      }
      if (!(*bullet).alive)
      {
        bullet = bullets.erase(bullet);
      }
      else
      {
        bullet++;
      }
    }
  }

  // Check Squib -> Bullet collisions
  for (auto &bullet : bullets)
  {
    for (auto &squib : squibs)
    {
      if (CheckCollisionCircles(Vector2{bullet.pos.x + 4, bullet.pos.y + 4}, 2, Vector2{squib.pos.x + 8, squib.pos.y + 8}, 8))
      {
        PlaySound(squib_hurt);
        squib.health -= 1;
        bullet.alive = false;
      }
    }
  }

  // Active shuttle pickup
  if (CheckCollisionPointCircle(player.pos, Vector2{pickup_pos.x + 4, pickup_pos.y + 12}, 64))
  {
    player.near_pickup = true;
    if (!start_pickup && IsKeyPressed(KEY_SPACE) && player.carbon == 50)
    {
      start_pickup = true;
      player.carbon -= 50;
      squib_spawn_time = 5;
      squib_spawn_timer = 0;
      max_squibs = 200;
      squib_spawn_range = Vector2{100, 300};
      squib_spawn_amount = Vector2{50, 200};
    }

    if (start_pickup && pickup_timer <= 0 && CheckCollisionPointRec(Vector2{player.pos.x + 4, player.pos.y + 4}, Rectangle{pickup_pos.x + 24 - 16, pickup_pos.y + 12 - 28, 32, 32}))
      ChangeState(GameState::GAME_WON);
  }
  else
  {
    player.near_pickup = false;
  }
}

void Game::Draw()
{
  BeginMode2D(camera);

  world.DrawMap(spritesheet, Rectangle{camera.target.x - (GAME_WIDTH / 2),
                                       camera.target.y - (GAME_HEIGHT / 2),
                                       GAME_WIDTH, GAME_HEIGHT});

  // Draw Tethers
  for (int i = 0; i < tethers.size(); i++)
  {
    Tether &tether = tethers[i];

    if (CheckCollisionPointCircle(Vector2{player.pos.x + 4, player.pos.y + 4}, tether.pos, tether.range))
    {
      DrawLineEx(tether.pos, Vector2{player.pos.x + 4, player.pos.y + 4}, 1.0f,
                 TETHER_COLOR);
    }

    for (int i = 1; i < tethers.size(); i++)
    {
      Tether &prev = tethers[i - 1];
      if (CheckCollisionPointCircle(prev.pos, tether.pos, tether.range))
      {
        DrawLineEx(tether.pos, prev.pos, 2.0f, TETHER_COLOR);
      }
    }
    DrawSprite(spritesheet, 12, tether.pos.x - 4, tether.pos.y - 4, 1.0f,
               WHITE);
    // DrawCircleLinesV(Vector2{tether.pos.x, tether.pos.y}, tether.range, TETHER_COLOR);
  }

  // Droppod drawing routines
  pod.Draw(spritesheet);

  // Draw Droppod flag
  DrawSprite(
      spritesheet,
      16,
      pickup_pos.x, pickup_pos.y - 8,
      1.0f, WHITE);
  DrawSprite(
      spritesheet,
      24,
      pickup_pos.x, pickup_pos.y,
      1.0f, WHITE);

  // Draw Pickup pod
  DrawTexturePro(
      spritesheet,
      Rectangle{8, 16, 32, 32},
      Rectangle{
          pickup_pos.x + 24, pickup_pos.y + 12,
          32, 32},
      Vector2{16, 28},
      0.0f, WHITE);

  DrawCircleLinesV(Vector2{pickup_pos.x + 4, pickup_pos.y + 12}, 64, BLUE);

  if (player.near_pickup && !start_pickup)
  {
    Vector2 size = MeasureTextEx(GetFontDefault(), "Call for pickup (-50 c)", 12, 1.0);
    DrawText(
        "Call for pickup (-50 c)",
        pickup_pos.x - (size.x / 2),
        pickup_pos.y + size.y,
        12,
        WHITE);
  }
  else if (start_pickup)
  {
    Vector2 size = MeasureTextEx(GetFontDefault(), TextFormat("Pickup Inbound %.0f", pickup_timer), 12, 1.0);
    DrawText(
        TextFormat("Pickup Inbound %.0f", pickup_timer),
        pickup_pos.x - (size.x / 2),
        pickup_pos.y + size.y,
        12,
        WHITE);
  }

  // Draw Squibs
  for (auto squib : squibs)
  {
    squib.Draw(spritesheet);
  }

  // Draw Player
  player.Draw(spritesheet);

  // Draw bullets
  for (auto bullet : bullets)
  {
    Vector2 screen_pos = GetWorldToScreen2D(bullet.pos, camera);
    float angle = atan2(screen_pos.y - (SCREEN_HEIGHT / 2), screen_pos.x - (SCREEN_WIDTH / 2)) * (180.0f / PI);
    DrawTexturePro(
        spritesheet, Rectangle{56, 8, CELL_SIZE, CELL_SIZE},
        Rectangle{bullet.pos.x, bullet.pos.y, CELL_SIZE, CELL_SIZE},
        Vector2{4, 4}, angle, WHITE);
  }

  // Flip gun if past angle
  float angle = atan2(GetMouseY() - (SCREEN_HEIGHT / 2), GetMouseX() - (SCREEN_WIDTH / 2)) * (180.0f / PI);
  float flip = 1;
  if (abs(angle) > 90)
  {
    flip = -1;
  }

  // Draw Gun
  DrawTexturePro(
      spritesheet,
      Rectangle{7 * CELL_SIZE, 0, CELL_SIZE, CELL_SIZE * flip},
      Rectangle{player.pos.x + 4, player.pos.y + 4, CELL_SIZE, CELL_SIZE},
      Vector2{4, 4},
      angle,
      WHITE);

  // Draw flag indicator
  Vector2 dir = Vector2Normalize(Vector2Subtract(Vector2{pickup_pos.x + 4, pickup_pos.y + 12}, Vector2{player.pos.x + 4, player.pos.y + 4}));
  DrawSprite(
      spritesheet,
      32,
      (player.pos.x + 4) + (dir.x * 10),
      (player.pos.y + 4) + (dir.y * 10),
      1.0f,
      WHITE);
  EndMode2D();

  if (paused)
  {
    DrawRectangle(0, 0, float(SCREEN_WIDTH), float(SCREEN_HEIGHT),
                  Color{0, 0, 0, 50});
    DrawText("PAUSED", 4, 4, 64, WHITE);
    if (GuiButton(Rectangle{4, SCREEN_HEIGHT - 48 - 72, 128, 48}, "Resume"))
    {
      paused = false;
    }
    if (GuiButton(Rectangle{4, SCREEN_HEIGHT - 48 - 128, 128, 48}, "Quit"))
    {
      ChangeState(GameState::MENU);
    }
  }
  else
  {
    // Draw UI
    GuiPanel(Rectangle{2, 2, 128, 86}, "Status");
    GuiLabel(Rectangle{4, 16, 128, 32},
             TextFormat("Health: %i", player.hp));
    GuiLabel(Rectangle{4, 32, 128, 32}, TextFormat("Carbon: %i", player.carbon));
    GuiLabel(Rectangle{4, 48, 128, 32}, TextFormat("Ammo: %i / %i", player.ammo, player.max_ammo));
    GuiLabel(Rectangle{4, 64, 128, 32},
             TextFormat("Tethers: %i", player.tethers));

    if (pod.showInfoPanel)
    {
      player.can_shoot = false;

      GuiPanel(Rectangle{2, 90, 128, 62}, "Drop Pod");
      GuiLabel(Rectangle{4, 114, 128, 14},
               TextFormat("Stored Carbon: %i", pod.carbon));

      if (GuiButton(Rectangle{4, 128, 124, 20}, "Deposit"))
      {
        pod.carbon += player.carbon;
        player.carbon = 0;
        PlaySound(deposit);
      }

      int result =
          GuiWindowBox(Rectangle{2, SCREEN_HEIGHT - (SCREEN_HEIGHT / 2), 128,
                                 (SCREEN_HEIGHT / 2) - 2},
                       "Crafting");

      // Draw tether buy button
      if (TextureButton(spritesheet, 12, Rectangle{4, 252, 124, 32},
                        "Tether\n(-10 C)"))
      {
        if (pod.carbon >= 10)
        {
          pod.carbon -= 10;
          player.tethers += 1;
          PlaySound(craft);
        }
      }

      // Draw ammo buy button
      if (TextureButton(spritesheet, 14, Rectangle{4, 252 + 34, 124, 32},
                        "Ammo x32\n(-5 C)"))
      {
        if (pod.carbon >= 5 && player.ammo < player.max_ammo)
        {
          player.ammo += 32;
          player.ammo = Clamp(player.ammo, 0, player.max_ammo);
          pod.carbon -= 5;
          PlaySound(craft);
        }
      }

      // Draw patch kit buy button
      if (TextureButton(spritesheet, 13, Rectangle{4, 252 + 68, 124, 32},
                        "Med Kit\n(-20 C)"))
      {
        if (pod.carbon >= 20 && player.hp < player.max_hp)
        {
          player.hp = player.max_hp;
          pod.carbon -= 20;
          PlaySound(craft);
        }
      }

      if (result > 0)
      {
        pod.showInfoPanel = false;
      }
    }
  }
}

void Game::Unload() { UnloadTexture(spritesheet); }
