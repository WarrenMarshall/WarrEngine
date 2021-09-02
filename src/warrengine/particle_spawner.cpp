
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

void Particle_Spawner::find_spawn_pos_for_new_particle( Particle* particle )
{
	switch( type )
	{
		case e_particle_spawner_type::point:
		{
			particle->pos = Vec2::zero;
			break;
		}

		case e_particle_spawner_type::box:
		{
			Bounding_Box spawn_area( { -( w / 2.f ), -( h / 2.f ) }, { ( w / 2.f ), ( h / 2.f ) } );
			particle->pos = spawn_area.get_random_spot();
			break;
		}

		case e_particle_spawner_type::in_circle:
		{
			particle->pos = Random::get_random_in_circle( radius );
			break;
		}

		case e_particle_spawner_type::on_circle:
		{
			particle->pos = Random::get_random_on_circle( radius );
			break;
		}
	}
}

void Particle_Spawner::parse_from_config_string( std::string_view value )
{
	Tokenizer tok( value, "," );

	type = g_engine->find_int_from_symbol( *tok.get_next_token() );
	assert( type > 0 );

	switch( type )
	{
		case e_particle_spawner_type::point:
		{
			break;
		}

		case e_particle_spawner_type::box:
		{
			w = Text_Parser::int_from_str( *tok.get_next_token() );
			h = Text_Parser::int_from_str( *tok.get_next_token() );
			break;
		}

		case e_particle_spawner_type::in_circle:
		case e_particle_spawner_type::on_circle:
		{
			radius = Text_Parser::float_from_str( *tok.get_next_token() );
			break;
		}
	}
}

}
