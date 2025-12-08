#include "playing_state.h"
#include "raylib.h"

static Camera3D camera;

void playing_state_load() {
    camera = (Camera3D){
        .position = (Vector3){ 0.0f, 10.0f, 10.0f },
        .target = (Vector3){ 0.0f, 0.0f, 0.0f },
        .up = (Vector3){ 0.0f, 1.0f, 0.0f },
        .fovy = 45.0f,
        .projection = CAMERA_PERSPECTIVE
    };
}

void playing_state_update() {}

void playing_state_render() {
    ClearBackground(BLACK);
    DrawText("Playing State", 32, 32, 48, WHITE);
}

void playing_state_unload() {

}