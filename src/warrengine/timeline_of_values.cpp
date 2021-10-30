
#include "master_pch.h"

namespace war
{

Timeline_Of_Values::Timeline_Of_Values( e_timeline_type type )
	: type( type )
{
	key_frames.clear();
}

Timeline_Of_Values* Timeline_Of_Values::clear_key_frames()
{
	key_frames.clear();
	return this;
}

Timeline_Of_Values* Timeline_Of_Values::add_key_frame( const Timeline_Of_Values::Key_Frame& kf )
{
	key_frames.push_back( kf );
	return this;
}

// figure out which key_frame is the one we are approaching next
// based on a percentage indicator of where we are on the timeline.

size_t Timeline_Of_Values::find_next_key_frame_idx_from_pct( f32 pct )
{
	size_t kf_next = 0;

	for( ; kf_next < key_frames.size() - 1; ++kf_next )
	{
		if( key_frames[ kf_next ].pct_marker > pct )
		{
			break;
		}
	}

	return kf_next;
}

f32 Timeline_Of_Values::get_float_value( f32 pct_on_timeline )
{
	auto kf_max = find_next_key_frame_idx_from_pct( pct_on_timeline );
	size_t kf_min = kf_max - 1;

	// the range of percentages within the min/max key frames
	f32 pct_range = key_frames[ kf_max ].pct_marker - key_frames[ kf_min ].pct_marker;

	// the pct we are at within the min/max key frames
	f32 pct_within = ( pct_on_timeline - key_frames[ kf_min ].pct_marker ) / pct_range;

	// compute the value represented by that pct_within within the min/max key frames

	f32 min_value = key_frames[ kf_min ].float_value;
	f32 max_value = key_frames[ kf_max ].float_value;

	return min_value + ( ( max_value - min_value ) * pct_within );
}

Color Timeline_Of_Values::get_color_value( f32 pct_on_timeline )
{
	auto kf_max = find_next_key_frame_idx_from_pct( pct_on_timeline );
	size_t kf_min = kf_max - 1;

	// the range of percentages within the min/max key frames
	f32 pct_range = key_frames[ kf_max ].pct_marker - key_frames[ kf_min ].pct_marker;

	// the pct we are at within the min/max key frames
	f32 pct_within = ( pct_on_timeline - key_frames[ kf_min ].pct_marker ) / pct_range;

	// compute the value represented by that pct_within within the min/max key frames

	Color min_value = key_frames[ kf_min ].color_value;
	Color max_value = key_frames[ kf_max ].color_value;

	return min_value + ( ( max_value - min_value ) * pct_within );
}

}
