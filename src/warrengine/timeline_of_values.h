
#pragma once

namespace war
{

struct Timeline_Of_Values final
{
	struct Key_Frame final
	{
		float_t pct_marker = 0.f;

		union
		{
			float_t float_value;
			Color color_value;
		};

		Key_Frame()
		{
		}
		Key_Frame( float_t pct_marker, float_t value )
			: pct_marker( pct_marker ), float_value( value )
		{
		}

		Key_Frame( float_t pct_marker, const Color& value )
			: pct_marker( pct_marker ), color_value( value )
		{

		}
	};

	std::vector<Timeline_Of_Values::Key_Frame> key_frames;
	e_timeline_type type = e_timeline_type::invalid;

	Timeline_Of_Values( e_timeline_type type );

	Timeline_Of_Values* clear_key_frames();
	Timeline_Of_Values* add_key_frame( const Timeline_Of_Values::Key_Frame& kf );

	size_t find_next_key_frame_idx_from_pct( float_t pct );

	float_t get_float_value( float_t pct_on_timeline );
	Color get_color_value( float_t pct_on_timeline );
};

}
