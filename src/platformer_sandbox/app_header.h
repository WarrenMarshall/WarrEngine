
#pragma once

#define GAME ((main_app*)game)

#define cl_player collision_layer::layer1
#define cl_world collision_layer::layer2

// ----------------------------------------------------------------------------

#include <game_engine.h>

#include "main_app.h"
#include "entities.h"
#include "layer_default.h"
