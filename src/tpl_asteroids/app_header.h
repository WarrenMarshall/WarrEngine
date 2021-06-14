
#include <warrengine.h>

static war::bit_flag_generator collision_bits = 1;

static const unsigned coll_all = ++collision_bits;
static const unsigned coll_player = ++collision_bits;
static const unsigned coll_bullet = ++collision_bits;
static const unsigned coll_enemy = ++collision_bits;
static const unsigned coll_teleport = ++collision_bits;

#include "entities.h"
#include "entity_components.h"

#include "scene_starfield.h"
#include "scene_gameplay.h"

#include "tpl_asteroids.h"
