
#pragma once

namespace war
{

struct Timeline_Values final
{
	std::vector<Timeline_Values_Key_Frame> key_frames;
	e_timeline_type_t type = e_timeline_type::invalid;

	Timeline_Values( e_timeline_type_t type );

	Timeline_Values* clear_key_frames();
	Timeline_Values* add_key_frame( const Timeline_Values_Key_Frame& kf );

	size_t find_next_key_frame_idx_from_pct( float pct );

	float get_float_value( float pct_on_timeline );
	Color get_color_value( float pct_on_timeline );
};

}
