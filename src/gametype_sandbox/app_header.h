
#pragma once

#define game static_cast<gametype_sandbox_app*>(base_game.get())

#include <game_engine.h>

// ----------------------------------------------------------------------------

constexpr e_collision_layer clayer_world = collision_layer::bit2;
constexpr e_collision_layer clayer_player = collision_layer::bit3;
constexpr e_collision_layer clayer_ball = clayer_player;
constexpr e_collision_layer clayer_paddle = clayer_player;
constexpr e_collision_layer clayer_player2 = collision_layer::bit4;
constexpr e_collision_layer clayer_coin = collision_layer::bit5;

// ----------------------------------------------------------------------------

#include "layer_main_menu.h"
#include "gametype_sandbox_app.h"

#include "platformer_entities.h"
#include "platformer_physics.h"
#include "layer_platformer.h"

#include "pong_entities.h"
#include "pong_physics.h"
#include "layer_pong.h"
