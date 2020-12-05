
#pragma once

#define game (static_cast<snake_pong*>(base_game.get()))

#include <warr_engine.h>

// ----------------------------------------------------------------------------

static w_bit_flag_generator clayer_bits( 2 );

static unsigned clayer_world_top_bottom = clayer_bits.next();
static unsigned clayer_world_left_right = clayer_bits.next();
static unsigned clayer_player_h = clayer_bits.next();
static unsigned clayer_player_v = clayer_bits.next();

// ----------------------------------------------------------------------------

struct layer_main_menu;
struct layer_game;

// ----------------------------------------------------------------------------

#include "entities.h"
#include "snake_pong.h"

#include "layer_background.h"
#include "layer_main_menu.h"
#include "layer_game.h"
