
#pragma once

namespace war
{

struct timeline_nodes_key_frame
{
	life_cycle_mgr life_cycle;
	float pct_marker = 0.f;

	timeline_nodes_key_frame() = default;
	timeline_nodes_key_frame( float pct_marker );

	virtual void update();
};

// ----------------------------------------------------------------------------

// temp - for testing
struct timeline_nkf_msg : timeline_nodes_key_frame
{
	std::string msg;	// temp

	timeline_nkf_msg( float pct_marker, std::string msg );

	virtual void update() override;
};

}
