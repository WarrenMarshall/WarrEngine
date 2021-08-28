
#include "master_pch.h"
#include "master_header.h"

namespace war
{

Timeline_Values_Key_Frame::Timeline_Values_Key_Frame( float pct_marker, float value )
	: pct_marker( pct_marker ), float_value( value )
{
}

Timeline_Values_Key_Frame::Timeline_Values_Key_Frame( float pct_marker, const Color& value )
	: pct_marker( pct_marker ), color_value( value )
{
}

}
