
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

e_player::e_player() 
{
	add_component<ec_sprite>()->init( this, game->tile_masters[ 31 ].subtex );
	add_component<ec_emitter>()->init( this, "emitter_debug" );
}

// ----------------------------------------------------------------------------
