
#pragma once

#define GAME ((gametype_sandbox_app*)game)

#include <game_engine.h>

// ----------------------------------------------------------------------------

constexpr unsigned IDF_None = 0;
constexpr unsigned IDF_PlayerFootSensor = 1;
constexpr unsigned IDF_PlayerDropSensor = 2;
constexpr unsigned IDF_World = 3;

// ----------------------------------------------------------------------------

constexpr e_collision_layer clayer_world = collision_layer::bit2;
constexpr e_collision_layer clayer_player = collision_layer::bit3;
constexpr e_collision_layer clayer_player2 = collision_layer::bit4;

// ----------------------------------------------------------------------------

#include "layer_main_menu.h"
#include "gametype_sandbox_app.h"

#include "platformer_entities.h"
#include "layer_platformer.h"

#include "layer_pong.h"
