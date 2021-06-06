
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

void w_particle_spawner::find_spawn_pos_for_new_particle( w_particle* particle )
{
	switch( type )
	{
		case particle_spawner_type::point:
		{
			particle->pos = w_vec2::zero;
			break;
		}

		case particle_spawner_type::box:
		{
			particle->pos.x = w_range( -( w / 2.0f ), ( w / 2.0f ) ).get_value();
			particle->pos.y = w_range( -( h / 2.0f ), ( h / 2.0f ) ).get_value();
			break;
		}

		case particle_spawner_type::circle:
		{
			particle->pos = engine->random->get_random_unit_circle() * ( radius * engine->random->getf() );
			break;
		}
	}
}

void w_particle_spawner::parse_from_config_string( std::string_view value )
{
	w_tokenizer tok( value, ',' );

	type = engine->find_int_from_symbol( *tok.get_next_token() );
	assert( type > 0 );

	switch( type )
	{
		case particle_spawner_type::point:
		{
			break;
		}

		case particle_spawner_type::box:
		{
			w = w_parser::int_from_str( *tok.get_next_token() );
			h = w_parser::int_from_str( *tok.get_next_token() );
			break;
		}

		case particle_spawner_type::circle:
		{
			radius = w_parser::float_from_str( *tok.get_next_token() );
			break;
		}
	}
}
