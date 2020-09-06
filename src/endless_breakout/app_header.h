
#pragma once

#define GAME ((endless_breakout_game*)game)

// ----------------------------------------------------------------------------

#define cl_ball				collision_layer::layer1
#define cl_paddle			collision_layer::layer2
#define cl_brick			collision_layer::layer3
#define cl_wall				collision_layer::layer4
#define cl_deathzone		collision_layer::layer5
#define cl_powup_fireball	collision_layer::layer6
#define cl_powup_multiball	collision_layer::layer7

// ----------------------------------------------------------------------------

#include <game_engine.h>

#include "entities.h"
#include "app_main.h"
#include "layer_background.h"
#include "layer_gameplay.h"
#include "layer_firetoplay.h"
