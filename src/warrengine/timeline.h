
#pragma once

namespace war
{

struct timeline
{
	std::vector<timeline_key_frame> key_frames;
	e_timeline_type type = timeline_type::invalid;

	timeline( e_timeline_type type );

	timeline* clear_key_frames();
	timeline* add_key_frame( const timeline_key_frame& kf );

	size_t find_next_key_frame_idx_from_pct( float pct );

	// computes a value on the timeline between 0-1, based on the
	// "pct_on_timeline" pass in.

	template<typename T>
	void get_value( float pct_on_timeline, T* value )
	{
		auto kf_max = find_next_key_frame_idx_from_pct( pct_on_timeline );
		size_t kf_min = kf_max - 1;

		// the range of percentages within the min/max key frames
		float pct_range = key_frames[ kf_max ].pct_marker - key_frames[ kf_min ].pct_marker;

		// the pct we are at within the min/max key frames
		float pct_within = ( pct_on_timeline - key_frames[ kf_min ].pct_marker ) / pct_range;

		// compute the value represented by that pct_within within the min/max key frames

		T min_value = std::get<T>( key_frames[ kf_min ].value );
		T max_value = std::get<T>( key_frames[ kf_max ].value );

		*value = min_value + ( ( max_value - min_value ) * pct_within );
	}
};

}
