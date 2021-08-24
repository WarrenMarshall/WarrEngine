
#pragma once

namespace war
{

struct timeline final
{
	std::vector<timeline_key_frame> key_frames;
	e_timeline_type type = timeline_type::invalid;

	timeline( e_timeline_type type );

	timeline* clear_key_frames();
	timeline* add_key_frame( const timeline_key_frame& kf );

	size_t find_next_key_frame_idx_from_pct( float pct );

	float get_float_value( float pct_on_timeline );
	color get_color_value( float pct_on_timeline );
};

}
