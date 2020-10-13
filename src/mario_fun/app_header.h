
#pragma once

#define game (static_cast<mario_fun_game*>(base_game.get()))

#include <game_engine.h>

// ----------------------------------------------------------------------------

static w_bit_flag_generator clayer_bits(2);

static unsigned clayer_world = clayer_bits.next();
static unsigned clayer_player = clayer_bits.next();
static unsigned clayer_ball = clayer_player;
static unsigned clayer_paddle = clayer_player;
static unsigned clayer_player2 = clayer_bits.next();
static unsigned clayer_coin = clayer_bits.next();
static unsigned clayer_mover = clayer_bits.next();

// ----------------------------------------------------------------------------

#include "layer_main_menu.h"
#include "mario_fun_game.h"

#include "platformer_entities.h"
#include "platformer_physics.h"
#include "platformer_layer.h"
