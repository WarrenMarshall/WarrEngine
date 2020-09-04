
#pragma once

#define GAME ((endless_breakout_game*)game)

// ----------------------------------------------------------------------------

#define cl_ball collision_layer::layer1
#define cl_paddle collision_layer::layer2
#define cl_brick collision_layer::layer3
#define cl_wall collision_layer::layer4

// ----------------------------------------------------------------------------

#include <game_engine.h>

#include "entities.h"
#include "endless_breakout.h"
#include "ui_styles.h"
#include "layer_background.h"
#include "layer_gameplay.h"
