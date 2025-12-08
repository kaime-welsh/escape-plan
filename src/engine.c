#include "engine.h"
#include "raylib.h"
#include "network.h"
#include "playing_state.h"

Engine engine_initialize(WindowConfig config)
{
    // Initialize engine structure
    Engine engine = {
        .windowConfig = config,
        .isRunning = true,
        .currentState = GAME_STATE_MENU,
        .previousState = GAME_STATE_MENU,
    };

    // Setup window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_MAXIMIZED);
    InitWindow(engine.windowConfig.width, engine.windowConfig.height, engine.windowConfig.title);
    SetTargetFPS(500);

    network_initialize();

    if (is_server)
    {
        network_host();
    }

    if (local_client)
    {
        // If this process has a local client, connect (loopback if also hosting)
        network_connect("127.0.0.1");
    }

    // Load initial state
    engine_change_state(&engine, GAME_STATE_PLAYING);
    return engine;
}

void engine_run(Engine *engine)
{
    engine->isRunning = true;
    while (engine->isRunning == true && !WindowShouldClose())
    {
        // Update network
        // TODO: Implement proper fixed timestep for network updates
        network_update();

        // Update current state
        switch (engine->currentState)
        {
        case GAME_STATE_MENU:
            break;
        case GAME_STATE_PLAYING:
            playing_state_update();
            break;
        case GAME_STATE_PAUSED:
            break;
        case GAME_STATE_GAMEOVER:
            break;
        default:
            break;
        }

        // Draw current state
        BeginDrawing();
        ClearBackground(BLACK);
        switch (engine->currentState)
        {
        case GAME_STATE_MENU:
            break;
        case GAME_STATE_PLAYING:
            playing_state_render();
            break;
        case GAME_STATE_PAUSED:
            break;
        case GAME_STATE_GAMEOVER:
            break;
        default:
            break;
        }
        EndDrawing();
    }
}

void engine_shutdown(Engine *engine)
{
    switch (engine->previousState)
    {
    case GAME_STATE_MENU:
        break;
    case GAME_STATE_PLAYING:
        playing_state_unload();
        break;
    case GAME_STATE_PAUSED:
        break;
    case GAME_STATE_GAMEOVER:
        break;
    default:
        break;
    }

    network_deinitialize();
    CloseWindow();
}

void engine_change_state(Engine *engine, enum GameState newState)
{
    // Unload previous state
    engine->previousState = engine->currentState;
    switch (engine->previousState)
    {
    case GAME_STATE_MENU:
        break;
    case GAME_STATE_PLAYING:
        playing_state_unload();
        break;
    case GAME_STATE_PAUSED:
        break;
    case GAME_STATE_GAMEOVER:
        break;
    default:
        break;
    }

    // Load new state
    engine->currentState = newState;
    switch (engine->currentState)
    {
    case GAME_STATE_MENU:
        break;
    case GAME_STATE_PLAYING:
        playing_state_load();
        break;
    case GAME_STATE_PAUSED:
        break;
    case GAME_STATE_GAMEOVER:
        break;
    default:
        break;
    }
}