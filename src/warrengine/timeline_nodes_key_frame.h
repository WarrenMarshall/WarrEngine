
#pragma once

namespace war
{

struct timeline_nodes_key_frame
{
	life_cycle_mgr life_cycle;
	float pct_marker = 0.f;
	bool should_restore_state = false;
	time_ms started = 0, duration = 0;

	// used to react to key_frames that start running
	bool is_running = false;

	timeline_nodes_key_frame() = default;
	timeline_nodes_key_frame( bool should_restore_state, float pct_marker, time_ms duration );

	virtual void on_started_running();
	virtual void update();
};

// ----------------------------------------------------------------------------

struct timeline_nkf_transform_shake_angle : timeline_nodes_key_frame
{
	struct
	{
		float angle = 0.f;
	} saved_state;

	noise_simplex noise = {};

	transform* tform = nullptr;
	float strength = 1.0f;

	timeline_nkf_transform_shake_angle( bool should_restore_state, float pct_marker, time_ms duration, transform* tform, float strength );

	virtual void on_started_running() override;
	virtual void update() override;
};

// ----------------------------------------------------------------------------

struct timeline_nkf_pp_color_overlay : timeline_nodes_key_frame
{
	struct
	{
		color color_overlay = {};
	} saved_state;


	color color_overlay = {};

	timeline_nkf_pp_color_overlay( bool should_restore_state, float pct_marker, time_ms duration, color color_overlay );

	virtual void on_started_running() override;
	virtual void update() override;
};

// ----------------------------------------------------------------------------

struct timeline_nkf_play_sound : timeline_nodes_key_frame
{
	struct
	{
	} saved_state;


	sound_asset* snd = nullptr;

	timeline_nkf_play_sound( bool should_restore_state, float pct_marker, time_ms duration, sound_asset* snd );

	virtual void on_started_running() override;
};

}
