
#include "master_pch.h"
#include "master_header.h"

namespace war
{

timeline_nodes_key_frame::timeline_nodes_key_frame( float pct_marker )
	: pct_marker( pct_marker )
{
}

void timeline_nodes_key_frame::update()
{
}

// ----------------------------------------------------------------------------

timeline_nkf_msg::timeline_nkf_msg( float pct_marker, std::string msg )
	: timeline_nodes_key_frame( pct_marker ),
	msg( msg )
{

}

void timeline_nkf_msg::update()
{
	timeline_nodes_key_frame::update();

	log( "{}", msg );
	set_life_cycle( life_cycle::dead );
}

}
