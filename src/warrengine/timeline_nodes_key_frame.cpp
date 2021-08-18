
#include "master_pch.h"
#include "master_header.h"

namespace war
{

timeline_nodes_key_frame::timeline_nodes_key_frame( bool should_restore_state, float pct_marker )
	: should_restore_state( should_restore_state ), pct_marker( pct_marker )
{
}

void timeline_nodes_key_frame::started_running()
{
}

void timeline_nodes_key_frame::update()
{
	if( !is_running )
	{
		is_running = true;
		started_running();
	}
}

// ----------------------------------------------------------------------------

timeline_nkf_msg::timeline_nkf_msg( bool should_restore_state, float pct_marker, std::string msg )
	: timeline_nodes_key_frame( should_restore_state, pct_marker ),
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
	: timeline_nodes_key_frame( should_restore_state, pct_marker ),
	tform( tform ), duration( duration ), strength( strength )
{
}

void timeline_nkf_transform_shake_angle::started_running()
{
	started = g_engine->time.now();
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

}
