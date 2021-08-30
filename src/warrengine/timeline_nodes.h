
#pragma once

namespace war
{

struct Timeline_Nodes final
{
	std::vector<Timeline_Nodes_Key_Frame> key_frames;
	Life_Cycle_Mgr life_cycle;
	time_ms start = 0, end = 0;
	time_ms duration = 0;

	Timeline_Nodes();
	~Timeline_Nodes() = default;

	void clear( time_ms duration );
	void go();
	void update();

	void add_kf_shake_angle( bool should_restore_state, float pct_marker, time_ms duration, Transform* tform, float strength );
	void add_kf_pp_color_overlay( bool should_restore_state, float pct_marker, time_ms duration, Color color );
	void add_kf_pp_pixelate( bool should_restore_state, float pct_marker, time_ms duration, float pixelate_factor );
	void add_kf_play_sound( bool should_restore_state, float pct_marker, Sound_Asset* snd );
	void add_kf_scene_push_under( bool should_restore_state, float pct_marker, std::unique_ptr<scene> scene_to_push );
	void add_kf_scene_pop_under( bool should_restore_state, float pct_marker );
};

}
