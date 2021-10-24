
#include <warrengine.h>

#include "robotron_app.h"

constexpr struct
{
	ui16 world			= 0b00000001;
	ui16 player			= 0b00000010;
	ui16 player_bullet	= 0b00000100;
} coll_flags;

#include "entities.h"

#include "scene_background.h"
#include "scene_menu.h"
#include "scene_gameplay.h"
