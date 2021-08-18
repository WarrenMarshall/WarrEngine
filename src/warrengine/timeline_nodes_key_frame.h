
#pragma once

namespace war
{

struct timeline_nodes_key_frame
{
	life_cycle_mgr life_cycle;
	float pct_marker = 0.f;
	bool should_restore_state = false;

	// used to react to key_frames that start running
	bool is_running = false;

	timeline_nodes_key_frame() = default;
	timeline_nodes_key_frame( bool should_restore_state, float pct_marker );

	virtual void started_running();
	virtual void update();
};

// ----------------------------------------------------------------------------

// temp - for testing
struct timeline_nkf_msg : timeline_nodes_key_frame
{
	std::string msg;	// temp

	timeline_nkf_msg( bool should_restore_state, float pct_marker, std::string msg );

	virtual void update() override;
};

// ----------------------------------------------------------------------------

struct timeline_nkf_transform_shake_angle : timeline_nodes_key_frame
{
	struct
	{
		float angle = 0.f;
	} saved_state;

	noise_simplex noise = {};

	time_ms started = 0, duration = 0;
	transform* tform = nullptr;
	float strength = 1.0f;

	timeline_nkf_transform_shake_angle( bool should_restore_state, float pct_marker, time_ms duration, transform* tform, float strength );

	virtual void started_running() override;
	virtual void update() override;
};

}
