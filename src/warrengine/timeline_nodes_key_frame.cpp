
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

timeline_nodes_key_frame::timeline_nodes_key_frame( e_tnkf_type type, bool should_restore_state, float pct_marker, time_ms duration )
	: type( type ), should_restore_state( should_restore_state ), pct_marker( pct_marker ), duration( duration )
{
}

void timeline_nodes_key_frame::on_started_running()
{
	started = g_engine->time.now();

	switch( type )
	{
		// ----------------------------------------------------------------------------
		case tnkf_type::shake_angle:
		{
			shake_angle.save.angle = shake_angle.tform->angle;
		}
		break;

		// ----------------------------------------------------------------------------
		case tnkf_type::pp_color_overlay:
		{
			auto clr = g_engine->render_api.get_uniform_color( "u_color_overlay" );
			pp_color_overlay.save.clr = clr;
		}
		break;

		// ----------------------------------------------------------------------------
		case tnkf_type::play_sound:
		{
			play_sound.snd->play();
			life_cycle.set( life_cycle::dead );
		}
		break;
	}
}

void timeline_nodes_key_frame::update()
{
	if( !is_running )
	{
		is_running = true;
		on_started_running();
	}

	switch( type )
	{
		// ----------------------------------------------------------------------------
		case tnkf_type::shake_angle:
		{
			if( g_engine->time.now() < started + duration )
			{
				shake_angle.tform->angle = shake_angle.save.angle + ( shake_angle.noise.get() * shake_angle.strength );
			}
			else
			{
				if( should_restore_state )
				{
					shake_angle.tform->angle = shake_angle.save.angle;
				}
				life_cycle.set( life_cycle::dead );
			}
		}
		break;

		// ----------------------------------------------------------------------------
		case tnkf_type::pp_color_overlay:
		{
			if( g_engine->time.now() < started + duration )
			{
				//color clr = { pp_color_overlay.r, pp_color_overlay.g, pp_color_overlay.b, pp_color_overlay.a };
				g_engine->render_api.set_uniform_color( "u_color_overlay", pp_color_overlay.clr );
			}
			else
			{
				if( should_restore_state )
				{
					g_engine->render_api.set_uniform_color( "u_color_overlay", pp_color_overlay.save.clr );
				}
				life_cycle.set( life_cycle::dead );
			}
		}
		break;
	}
}

}
