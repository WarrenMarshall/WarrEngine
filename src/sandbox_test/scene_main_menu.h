
using namespace war;

struct scene_main_menu final : scene
{
	scene_main_menu();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	template<typename T>
	void do_menu_button( hash tag, const char* text )
	{
		if( g_ui
			->button_control()
			->set_slice_def( "" )
			->set_text( text )
			->cut_top( 12.f )
			->done() )
		{
			Timeline_Nodes tl;
			tl.clear( 1000 );
			tl.add_kf_scene_pop_under( false, 0.5f );
			tl.add_kf_pp_color_overlay( true, 0.0f, 1000, make_color( Color::red, 0.5f ) );
			tl.add_kf_scene_push_under( false, 0.5f, std::make_unique<T>() );

			g_engine->scenes.do_transient_timeline( tl );
		}
	};
};
