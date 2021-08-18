
#pragma once

namespace war
{

struct timeline_nodes_key_frame
{
	float pct_marker = 0.f;

	timeline_nodes_key_frame() = default;
	timeline_nodes_key_frame( float pct_marker );

	// #cleanup - if these are going to do nothing, just remove them and make life_cycle public
	void set_life_cycle( e_life_cycle lc ) { life_cycle.set( lc ); }
	const life_cycle_mgr* get_life_cycle() { return &life_cycle; }

	virtual void update();

private:
	life_cycle_mgr life_cycle;
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
