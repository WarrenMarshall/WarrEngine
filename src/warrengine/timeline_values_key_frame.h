
#pragma once

namespace war
{

struct Timeline_Values_Key_Frame
{
	float_t pct_marker = 0.f;

	union
	{
		float_t float_value;
		Color color_value;
	};

	Timeline_Values_Key_Frame() {}
	Timeline_Values_Key_Frame( float_t pct_marker, float_t value );
	Timeline_Values_Key_Frame( float_t pct_marker, const Color& value );
};

}
