
#pragma once

namespace war
{

struct timeline_nodes_key_frame
{
	e_tnkf_type type = tnkf_type::none;
	life_cycle_mgr life_cycle;
	float pct_marker = 0.f;
	bool should_restore_state = false;
	time_ms started = 0, duration = 0;

	// used to react to key_frames that start running
	bool is_running = false;

	union
	{
		// ----------------------------------------------------------------------------
		struct
		{
			struct
			{
				float angle;
			} save;

			noise_simplex noise;
			transform* tform;
			float strength;
		} shake_angle;

		// ----------------------------------------------------------------------------
		struct
		{
			struct
			{
				color clr;
			} save;

			color clr;
		} pp_color_overlay;

		// ----------------------------------------------------------------------------
		struct
		{
			sound_asset* snd;
		} play_sound;
	};

	timeline_nodes_key_frame( e_tnkf_type type, bool should_restore_state, float pct_marker, time_ms duration );

	void on_started_running();
	void update();
};

}
