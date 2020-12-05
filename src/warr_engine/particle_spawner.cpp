
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

void w_particle_spawner::find_spawn_pos_for_new_particle( w_particle* particle )
{
	particle->pos = w_vec2::zero;
}

void w_particle_spawner::parse_from_config_string( std::string_view value )
{
}

// ----------------------------------------------------------------------------

w_particle_spawner_box::w_particle_spawner_box( const w_particle_spawner_box& other )
{
	w = other.w;
	h = other.h;
}

w_particle_spawner_box::w_particle_spawner_box( int w, int h )
	: w( w ), h( h )
{
}

void w_particle_spawner_box::find_spawn_pos_for_new_particle( w_particle* particle )
{
	particle->pos.x = w_range( -( w / 2.0f ), ( w / 2.0f ) ).get_value();
	particle->pos.y = w_range( -( h / 2.0f ), ( h / 2.0f ) ).get_value();
}

void w_particle_spawner_box::parse_from_config_string( std::string_view value )
{
	w_tokenizer tok( value, ',' );
	tok.get_next_token();	// throw away the type
	w = w_parser::int_from_str( *tok.get_next_token() );
	h = w_parser::int_from_str( *tok.get_next_token() );
}

// ----------------------------------------------------------------------------

w_particle_spawner_circle::w_particle_spawner_circle( const w_particle_spawner_circle& other )
	: radius( other.radius )
{
}

w_particle_spawner_circle::w_particle_spawner_circle( float radius )
	: radius( radius )
{
}

void w_particle_spawner_circle::find_spawn_pos_for_new_particle( w_particle* particle )
{
	// getting random values between -1 and 1 ... this gives us a position on the outer
	// edge of the unit circle
	particle->pos = { engine->random->getf() - 0.5f, engine->random->getf() - 0.5f };
	particle->pos.normalize();

	// then multiply against a random value from 0-radius to place
	// the particle somewhere within the spawn zone
	particle->pos.x *= radius * engine->random->getf();
	particle->pos.y *= radius * engine->random->getf();
}

void w_particle_spawner_circle::parse_from_config_string( std::string_view value )
{
	w_tokenizer tok( value, ',' );
	tok.get_next_token();	// throw away the type
	radius = w_parser::float_from_str( *tok.get_next_token() );
}

