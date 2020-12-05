
#pragma once

#define game (static_cast<snake_pong*>(base_game.get()))

// ----------------------------------------------------------------------------

struct layer_main_menu;
struct layer_game;

// ----------------------------------------------------------------------------

#include <warr_engine.h>

#include "snake_pong.h"

#include "layer_background.h"
#include "layer_main_menu.h"
#include "layer_game.h"
