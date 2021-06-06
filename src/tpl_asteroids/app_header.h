
#include <warrengine.h>

static war::bit_flag_generator collision_bits( 1 );

static const unsigned coll_all = collision_bits.next();
static const unsigned coll_player = collision_bits.next();
static const unsigned coll_bullet = collision_bits.next();
static const unsigned coll_enemy = collision_bits.next();
static const unsigned coll_teleport = collision_bits.next();

#include "entities.h"
#include "entity_components.h"

#include "scene_starfield.h"
#include "scene_gameplay.h"

#include "tpl_asteroids.h"
