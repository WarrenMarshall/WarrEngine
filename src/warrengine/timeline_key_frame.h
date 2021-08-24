
#pragma once

namespace war
{

struct timeline_key_frame final
{
	float pct_marker = 0.f;

	union
	{
		float float_value;
		color color_value;
	};

	timeline_key_frame() {}
	timeline_key_frame( float pct_marker, float value );
	timeline_key_frame( float pct_marker, const color& value );
};

}
