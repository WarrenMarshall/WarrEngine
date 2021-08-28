
#pragma once

namespace war
{

struct Timeline_Values_Key_Frame final
{
	float pct_marker = 0.f;

	union
	{
		float float_value;
		Color color_value;
	};

	Timeline_Values_Key_Frame() {}
	Timeline_Values_Key_Frame( float pct_marker, float value );
	Timeline_Values_Key_Frame( float pct_marker, const Color& value );
};

}
