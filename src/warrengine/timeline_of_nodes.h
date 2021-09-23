
#pragma once

namespace war
{

struct Timeline_Of_Nodes final
{
	struct Key_Frame final
	{
		e_tnkf_type type = e_tnkf_type::none;
		Life_Cycle life_cycle;
		float_t pct_marker = 0.f;
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
					float_t angle;
				} save;

				Simplex_Noise noise;
				Transform* tform;
				float_t strength;
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
					float_t pixelate_factor;
				} save;

				float_t pixelate_factor;
			} pp_pixelate;

			// ----------------------------------------------------------------------------
			struct
			{
				Sound_Asset* snd;
			} play_sound;

			// ----------------------------------------------------------------------------
			struct
			{
				Scene* new_scene;
			} scene_push_under;

			// ----------------------------------------------------------------------------
			struct
			{
			} scene_pop;
		};

		Key_Frame( e_tnkf_type type, bool should_restore_state, float_t pct_marker, time_ms duration );

		void on_started_running();
		void update();
	};

	std::vector<Timeline_Of_Nodes::Key_Frame> key_frames;
	Life_Cycle life_cycle;
	time_ms start = 0, end = 0;
	time_ms duration = 0;

	Timeline_Of_Nodes();
	~Timeline_Of_Nodes() = default;

	void clear( time_ms duration );
	void go();
	void update();

	void add_kf_shake_angle( bool should_restore_state, float_t pct_marker, time_ms duration, Transform* tform, float_t strength );
	void add_kf_pp_color_overlay( bool should_restore_state, float_t pct_marker, time_ms duration, Color color );
	void add_kf_pp_pixelate( bool should_restore_state, float_t pct_marker, time_ms duration, float_t pixelate_factor );
	void add_kf_play_sound( bool should_restore_state, float_t pct_marker, const Sound_Asset* snd );
	void add_kf_scene_push_under( bool should_restore_state, float_t pct_marker, std::unique_ptr<Scene> scene_to_push );
	void add_kf_scene_pop_under( bool should_restore_state, float_t pct_marker );
	void add_kf_reset_scene_stack_to_main_menu( bool should_restore_state, float_t pct_marker );
};

}
