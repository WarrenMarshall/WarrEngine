
#pragma once

#define GAME ((endless_adventure_game*)game)

// ----------------------------------------------------------------------------

#include <game_engine.h>

// ----------------------------------------------------------------------------

enum_begin( room_layer )
	nobrowse = 0,
	geometry = 1,
	item = 2,
	enemy = 3,
	max
enum_end

// ----------------------------------------------------------------------------

#include "endless_adventure_game.h"

#include "ui_styles.h"

#include "layer_background.h"
#include "layer_browser.h"
#include "layer_editor.h"
#include "layer_worldviewport.h"
