
#include "app_header.h"

// ----------------------------------------------------------------------------

/*
player_ship_component::player_ship_component( entity* parent_entity )
	: entity_component( parent_entity )
{
}

void player_ship_component::draw()
{
	scoped_opengl;
	scoped_render_state;

	render::state->set_from_opt( rs_opt );
	render::state->color = make_color( pal::brightest );

	auto player = static_cast<e_player_ship*>( parent_entity );

	render::state->glow = 2.f;
	render::state->color.a = 0.25f;
	render::draw_lines( my_game->ship_geo[ my_game->ship_power_level ].verts );

	render::state->glow = 4.f;
	render::state->color.a = 1.f;
	for( auto& vtx : my_game->ship_geo[ my_game->ship_power_level ].verts )
	{
		render::draw_point( vtx );
	}
}
*/

// ----------------------------------------------------------------------------

/*
player_bullet_component::player_bullet_component( entity* parent_entity )
	: entity_component( parent_entity )
{
}

void player_bullet_component::draw()
{
	scoped_opengl;
	scoped_render_state;

	render::state->set_from_opt( rs_opt );
	render::state->color = make_color( pal::brightest );
	render::state->glow = 1.5f;
	render::state->color.a = 0.15f;

	render::draw_line( { 0.f, -3.f }, { 0.f, +3.f } );

	render::state->color.a = 1.f;
	render::draw_point( { 0.f, -3.f } );
}
*/
