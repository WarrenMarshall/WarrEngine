
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

e_player::e_player()
	: w_entity()
{
	add_component<ec_sprite>()->init( game->tile_masters[ 31 ].subtexture );
	add_component<ec_emitter>()->init( this, "emitter_debug" );
}

// ----------------------------------------------------------------------------

e_vfx_asteroid_hit::e_vfx_asteroid_hit()
	: w_entity_cozy()
{
	add_component<ec_emitter>()->init( this, "fireball_hit" );
	add_component<ec_sound>()->init( "player_hit" );
}
