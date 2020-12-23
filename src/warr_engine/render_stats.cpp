#include "master_pch.h"
#include "master_header.h"

w_render_stats::w_render_stats()
{
	stat_timer = std::make_unique<w_timer>( 1000 );
}

void w_render_stats::update()
{
	if( stat_timer->is_elapsed() )
	{
		stat_timer->reset();

		frame_count.update_value();
		auto steps = static_cast<int>( frame_count.value );

		frame_times_ms.update_value( steps );
		draw_calls.update_value( steps );
		vertices.update_value( steps );
		indices.update_value( steps );
		entities.update_value( steps );
	}
	else
	{
		stat_timer->update();
	}
}
