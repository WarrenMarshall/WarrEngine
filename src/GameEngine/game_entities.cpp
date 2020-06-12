
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

e_player::e_player()
	: w_entity()
{
	add_component<c_sprite>()->init( "player" );
	add_component<c_sprite>()->init( "player_shield" );
	//draw_bounds = true;
}

void e_player::draw()
{
	w_entity::draw();

	//engine->render->draw_circle( w_vec3( 0, 0, 0 ), 16 );

	// todo : this needs to be a in a static draw buffer
	//engine->render->draw_line(w_vec3(0, 8, 0), w_vec3(-4, -8, 0));
	//engine->render->draw_line( w_vec3( 0, 8, 0 ), w_vec3( +4, -8, 0 ) );
	//engine->render->draw_line( w_vec3( -4, -6, 0 ), w_vec3( +4, -6, 0 ) );
}

// ----------------------------------------------------------------------------

e_asteroid::e_asteroid()
	: w_entity()
{
	add_component<c_sprite>()->init( "fireball" );

	timer_flip_fireball = std::make_unique<w_timer>( 150 );
}

void e_asteroid::update()
{
	w_entity::update();

	pos.x += ( dir.x * speed ) * engine->time->delta_s;
	pos.y += ( dir.y * speed ) * engine->time->delta_s;

}

// ----------------------------------------------------------------------------

e_vfx_asteroid_hit::e_vfx_asteroid_hit()
	: w_entity_cozy()
{
	add_component<c_emitter>()->init( this, "fireball_hit" );
	add_component<c_sound>()->init( "player_hit" );
}
