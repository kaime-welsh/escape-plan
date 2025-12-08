#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdbool.h>

// Type Definitions
enum GameState {
    GAME_STATE_MENU,
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSED,
    GAME_STATE_GAMEOVER
};

typedef struct window_config_t{
    int width;
    int height;
    const char* title;
} WindowConfig;

typedef struct engine_t {
    WindowConfig windowConfig;
    bool isRunning;
    enum GameState currentState;
    enum GameState previousState;
} Engine;

// Engine Util Functions
void engine_change_state(Engine* engine, enum GameState newState);

#endif // GLOBAL_H