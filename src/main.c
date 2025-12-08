#include "engine.h"
#include <stdio.h>
#include <string.h>

bool is_server = false;

int main(int argc, char **argv) {
    WindowConfig config = {
        .width = 800,
        .height = 600,
        .title = "Escape Plan - Client"
    };

    if (argc > 1) {
        if (strcmp(argv[1], "server") == 0) {
            is_server = true;
            config.title = "Escape Plan - Server";
        }
    }

    Engine engine = engine_initialize(config);
    engine_run(&engine);
}