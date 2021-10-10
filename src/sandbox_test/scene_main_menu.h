
using namespace war;

struct Scene_Main_Menu final : Scene
{
	Scene_Main_Menu();

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
			Timeline_Of_Nodes tl;
			tl.init( 1000 );

			tl.add_kf_pp_color_overlay( false, 0.25f, 250, make_color( Color::black, 1.f ) );
			tl.add_kf_pp_pixelate( false, 0.0f, 500, 32.f );
			tl.add_kf_scene_push_under( false, 0.5f, std::make_unique<T>() );
			tl.add_kf_pp_pixelate( false, 0.5f, 500, 0.f );
			tl.add_kf_pp_color_overlay( false, 0.5f, 250, make_color( Color::black, 0.f ) );

			g_engine->scene_mgr.do_transient_timeline( tl );
		}
	};
};
