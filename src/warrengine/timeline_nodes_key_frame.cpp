
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

Timeline_Nodes_Key_Frame::Timeline_Nodes_Key_Frame( e_tnkf_type_t type, bool should_restore_state, float pct_marker, time_ms duration )
	: type( type ), should_restore_state( should_restore_state ), pct_marker( pct_marker ), duration( duration )
{
}

void Timeline_Nodes_Key_Frame::on_started_running()
{
	started = g_engine->clock.now();

	switch( type )
	{
		// ----------------------------------------------------------------------------
		case e_tnkf_type::shake_angle:
		{
			shake_angle.save.angle = shake_angle.tform->angle;
		}
		break;

		// ----------------------------------------------------------------------------
		case e_tnkf_type::pp_color_overlay:
		{
			pp_color_overlay.save.color = g_engine->opengl_mgr.get_uniform_color( "u_color_overlay" );
		}
		break;

		// ----------------------------------------------------------------------------
		case e_tnkf_type::play_sound:
		{
			play_sound.snd->play();
			life_cycle.set( e_life_cycle::dead );
		}
		break;

		// ----------------------------------------------------------------------------
		case e_tnkf_type::scene_push_under:
		{
			g_engine->scene_mgr.push_under( scene_push_under.new_scene );
			life_cycle.set( e_life_cycle::dead );
		}
		break;

		// ----------------------------------------------------------------------------
		case e_tnkf_type::scene_pop_under:
		{
			g_engine->scene_mgr.pop_under();
			life_cycle.set( e_life_cycle::dead );
		}
		break;
	}
}

void Timeline_Nodes_Key_Frame::update()
{
	if( !is_running )
	{
		is_running = true;
		on_started_running();
	}

	auto time_now = g_engine->clock.now();
	auto pct = ( time_now - started ) / (float)duration;

	switch( type )
	{
		// ----------------------------------------------------------------------------
		case e_tnkf_type::shake_angle:
		{
			if( time_now < started + duration )
			{
				shake_angle.tform->angle = shake_angle.save.angle + ( shake_angle.noise.get() * shake_angle.strength );
			}
			else
			{
				if( should_restore_state )
				{
					shake_angle.tform->angle = shake_angle.save.angle;
				}
				life_cycle.set( e_life_cycle::dead );
			}
		}
		break;

		// ----------------------------------------------------------------------------
		case e_tnkf_type::pp_color_overlay:
		{
			if( time_now < started + duration )
			{
				g_engine->opengl_mgr.set_uniform_color( "u_color_overlay",
					make_color( pp_color_overlay.color, lerp( pp_color_overlay.save.color.a, pp_color_overlay.color.a, pct ) )
				);
			}
			else
			{
				if( should_restore_state )
				{
					g_engine->opengl_mgr.set_uniform_color( "u_color_overlay", pp_color_overlay.save.color );
				}
				life_cycle.set( e_life_cycle::dead );
			}
		}
		break;
	}
}

}
