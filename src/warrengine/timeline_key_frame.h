
#pragma once

namespace war
{

struct timeline_key_frame
{
	float pct_marker = 0.f;
	std::variant<float, color> value = {};

	timeline_key_frame() = default;
	timeline_key_frame( float pct_marker, float value );
	timeline_key_frame( float pct_marker, const color& value );
};

}
