
#pragma once

namespace war
{

struct Timeline_Nodes_Key_Frame final
{
	e_tnkf_type_t type = e_tnkf_type::none;
	Life_Cycle_Mgr life_cycle;
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

			Simplex_Noise noise;
			Transform* tform;
			float strength;
		} shake_angle;

		// ----------------------------------------------------------------------------
		struct
		{
			struct
			{
				Color color;
			} save;

			Color color;
		} pp_color_overlay;

		// ----------------------------------------------------------------------------
		struct
		{
			struct
			{
				float pixelate_factor;
			} save;

			float pixelate_factor;
		} pp_pixelate;

		// ----------------------------------------------------------------------------
		struct
		{
			Sound_Asset* snd;
		} play_sound;

		// ----------------------------------------------------------------------------
		struct
		{
			scene* new_scene;
		} scene_push_under;

		// ----------------------------------------------------------------------------
		struct
		{
		} scene_pop;
	};

	Timeline_Nodes_Key_Frame( e_tnkf_type_t type, bool should_restore_state, float pct_marker, time_ms duration );

	void on_started_running();
	void update();
};

}
