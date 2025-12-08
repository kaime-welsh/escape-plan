#ifndef ENGINE_H
#define ENGINE_H

#include "global.h"

extern bool is_server;

Engine engine_initialize(WindowConfig config);
void engine_run(Engine* engine);
void engine_shutdown(Engine* engine);

#endif // ENGINE_H