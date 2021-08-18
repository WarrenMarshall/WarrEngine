
#include "master_pch.h"
#include "master_header.h"

namespace war
{

timeline_nodes::timeline_nodes()
{
	life_cycle.set( life_cycle::dead );
}

void timeline_nodes::clear()
{
	key_frames.clear();
}

void timeline_nodes::init( time_ms duration )
{
	this->duration = (float)duration;
	start = g_engine->time.now();
	end = start + duration;
	life_cycle.set( life_cycle::alive );
}

void timeline_nodes::update()
{
	if( key_frames.empty() )
	{
		return;
	}

	if( !life_cycle.is_alive() )
	{
		return;
	}

	float pct_on_timeline = glm::clamp( ( g_engine->time.now() - start ) / duration, 0.f, 1.f );

	// for every node_key_frame that is behind the current pct marker of this
	// timeline, call it's update() function.

	for( auto& kf : key_frames )
	{
		if( kf->life_cycle.is_alive() and kf->pct_marker <= pct_on_timeline )
		{
			kf->update();
		}
	}

	if( fequals( pct_on_timeline, 1.f ) )
	{
		life_cycle.set( life_cycle::dead );
	}
}

}
