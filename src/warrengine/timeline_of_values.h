
#pragma once

namespace war
{

struct Timeline_Of_Values final
{
	struct Key_Frame final
	{
		f32 pct_marker = 0.f;

		union
		{
			f32 float_value;
			Color color_value;
		};

		Key_Frame()
		{
		}
		Key_Frame( f32 pct_marker, f32 value )
			: pct_marker( pct_marker ), float_value( value )
		{
		}

		Key_Frame( f32 pct_marker, const Color& value )
			: pct_marker( pct_marker ), color_value( value )
		{

		}
	};

	std::vector<Timeline_Of_Values::Key_Frame> key_frames;
	e_timeline_type type = e_timeline_type::invalid;

	Timeline_Of_Values( e_timeline_type type );

	Timeline_Of_Values* clear_key_frames();
	Timeline_Of_Values* add_key_frame( const Timeline_Of_Values::Key_Frame& kf );

	size_t find_next_key_frame_idx_from_pct( f32 pct );

	f32 get_float_value( f32 pct_on_timeline );
	Color get_color_value( f32 pct_on_timeline );
};

}
