#include "engine.h"
#include <stdio.h>
#include <string.h>

bool is_server = false;
bool local_client = true;

int main(int argc, char **argv) {
    WindowConfig config = {
        .width = 800,
        .height = 600,
        .title = "Escape Plan - Client"
    };

    if (argc > 1) {
        if (strcmp(argv[1], "server") == 0) {
            is_server = true;
            local_client = false; // server-only
            config.title = "Escape Plan - Server";
        } else if (strcmp(argv[1], "host") == 0) {
            // host: server + local client
            is_server = true;
            local_client = true;
            config.title = "Escape Plan - Host";
        }
    }

    Engine engine = engine_initialize(config);
    engine_run(&engine);
}