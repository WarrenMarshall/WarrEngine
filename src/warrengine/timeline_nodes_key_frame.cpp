
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

timeline_nodes_key_frame::timeline_nodes_key_frame( bool should_restore_state, float pct_marker, time_ms duration )
	: should_restore_state( should_restore_state ), pct_marker( pct_marker ), duration( duration )
{
}

void timeline_nodes_key_frame::on_started_running()
{
	started = g_engine->time.now();
}

void timeline_nodes_key_frame::update()
{
	if( !is_running )
	{
		is_running = true;
		on_started_running();
	}
}

// ----------------------------------------------------------------------------

timeline_nkf_msg::timeline_nkf_msg( bool should_restore_state, float pct_marker, time_ms duration, std::string msg )
	: timeline_nodes_key_frame( should_restore_state, pct_marker, duration ),
	msg( msg )
{

}

void timeline_nkf_msg::update()
{
	timeline_nodes_key_frame::update();

	log( "{}", msg );
	life_cycle.set( life_cycle::dead );
}

// ----------------------------------------------------------------------------

timeline_nkf_transform_shake_angle::timeline_nkf_transform_shake_angle( bool should_restore_state, float pct_marker, time_ms duration, transform* tform, float strength )
	: timeline_nodes_key_frame( should_restore_state, pct_marker, duration ),
	tform( tform ), strength( strength )
{
}

void timeline_nkf_transform_shake_angle::on_started_running()
{
	timeline_nodes_key_frame::on_started_running();

	saved_state.angle = tform->angle;
}

void timeline_nkf_transform_shake_angle::update()
{
	timeline_nodes_key_frame::update();

	if( g_engine->time.now() < started + duration )
	{
		tform->angle = saved_state.angle + ( noise.get() * strength );
	}
	else
	{
		if( should_restore_state )
		{
			tform->angle = saved_state.angle;
		}
		life_cycle.set( life_cycle::dead );
	}
}

// ----------------------------------------------------------------------------

timeline_nkf_pp_color_overlay::timeline_nkf_pp_color_overlay( bool should_restore_state, float pct_marker, time_ms duration, color color_overlay )
	: timeline_nodes_key_frame( should_restore_state, pct_marker, duration ),
	color_overlay( color_overlay )
{
}

void timeline_nkf_pp_color_overlay::on_started_running()
{
	timeline_nodes_key_frame::on_started_running();

	saved_state.color_overlay = g_engine->render_api.get_uniform_color( "u_color_overlay" );
}

void timeline_nkf_pp_color_overlay::update()
{
	timeline_nodes_key_frame::update();

	if( g_engine->time.now() < started + duration )
	{
		g_engine->render_api.set_uniform_color( "u_color_overlay", color_overlay );
	}
	else
	{
		if( should_restore_state )
		{
			g_engine->render_api.set_uniform_color( "u_color_overlay", saved_state.color_overlay );
		}
		life_cycle.set( life_cycle::dead );
	}
}

}
