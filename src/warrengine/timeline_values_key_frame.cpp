
#include "master_pch.h"
#include "master_header.h"

namespace war
{

Timeline_Values_Key_Frame::Timeline_Values_Key_Frame( float_t pct_marker, float_t value )
	: pct_marker( pct_marker ), float_value( value )
{
}

Timeline_Values_Key_Frame::Timeline_Values_Key_Frame( float_t pct_marker, const Color& value )
	: pct_marker( pct_marker ), color_value( value )
{
}

}
