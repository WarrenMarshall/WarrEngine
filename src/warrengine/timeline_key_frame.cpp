
#include "master_pch.h"
#include "master_header.h"

namespace war
{

timeline_key_frame::timeline_key_frame( float pct_marker, float value )
	: pct_marker( pct_marker ), value( value )
{
}

timeline_key_frame::timeline_key_frame( float pct_marker, const color& value )
	: pct_marker( pct_marker ), value( value )
{
}

}
