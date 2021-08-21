
#pragma once

namespace war
{

struct timeline_nodes
{
	std::vector<timeline_nodes_key_frame> key_frames;
	life_cycle_mgr life_cycle;
	time_ms start = 0, end = 0;
	time_ms duration = 0;

	timeline_nodes();

	void clear( time_ms duration );
	void go();
	void update();

	void add_kf_shake_angle( bool should_restore_state, float pct_marker, time_ms duration, transform* tform, float strength );
	void add_kf_pp_color_overlay( bool should_restore_state, float pct_marker, time_ms duration, color color_overlay );
	void add_kf_play_sound( bool should_restore_state, float pct_marker, time_ms duration, sound_asset* snd );
};

}
