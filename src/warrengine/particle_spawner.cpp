
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

void particle_spawner::find_spawn_pos_for_new_particle( particle* particle )
{
	switch( type )
	{
		case particle_spawner_type::point:
		{
			particle->pos = vec2::zero;
			break;
		}

		case particle_spawner_type::box:
		{
			particle->pos.x = range( -( w / 2.f ), ( w / 2.f ) ).get_random_value();
			particle->pos.y = range( -( h / 2.f ), ( h / 2.f ) ).get_random_value();
			break;
		}

		case particle_spawner_type::in_circle:
		{
			particle->pos = random::get_random_in_circle( radius );
			break;
		}

		case particle_spawner_type::on_circle:
		{
			particle->pos = random::get_random_on_circle( radius );
			break;
		}
	}
}

void particle_spawner::parse_from_config_string( std::string_view value )
{
	tokenizer tok( value, "," );

	type = g_engine->find_int_from_symbol( *tok.get_next_token() );
	assert( type > 0 );

	switch( type )
	{
		case particle_spawner_type::point:
		{
			break;
		}

		case particle_spawner_type::box:
		{
			w = text_parser::int_from_str( *tok.get_next_token() );
			h = text_parser::int_from_str( *tok.get_next_token() );
			break;
		}

		case particle_spawner_type::in_circle:
		case particle_spawner_type::on_circle:
		{
			radius = text_parser::float_from_str( *tok.get_next_token() );
			break;
		}
	}
}

}
