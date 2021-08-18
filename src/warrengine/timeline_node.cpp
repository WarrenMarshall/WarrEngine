
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

timeline_key_frame::timeline_key_frame( float pct_marker, float value )
	: pct_marker( pct_marker ), value( value )
{
}

timeline_key_frame::timeline_key_frame( float pct_marker, const color& value )
	: pct_marker( pct_marker ), value( value )
{
}

// ----------------------------------------------------------------------------

timeline::timeline( e_timeline_type type )
	: type( type )
{
	key_frames.clear();
}

timeline* timeline::clear_key_frames()
{
	key_frames.clear();
	return this;
}

timeline* timeline::add_key_frame( const timeline_key_frame& kf )
{
	key_frames.push_back( kf );
	return this;
}

// figure out which key_frame is the one we are approaching next
// based on a percentage indicator of where we are on the timeline.

size_t timeline::find_next_key_frame_idx_from_pct( float pct )
{
	size_t kf_next = 0;

	for( ; kf_next < key_frames.size() - 1; ++kf_next )
	{
		if( key_frames[ kf_next ].pct_marker > pct )
		{
			break;
		}
	}

	return kf_next;
}

}
