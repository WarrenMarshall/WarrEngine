
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

UI_Control_Data* Scene_Wrapped_Text_UI_Callback::get_data( hash tag )
{
	switch( tag )
	{
		case H( "rb_align_left" ):
		case H( "rb_align_center" ):
		case H( "rb_align_right" ):
		{
			return &radio_alignment_data;
		}
		break;
	}

	return UI_Callback::get_data( tag );
}

// ----------------------------------------------------------------------------

Scene_Wrapped_Text::Scene_Wrapped_Text()
{
	ui_callback = std::make_unique<Scene_Wrapped_Text_UI_Callback>();

	flags.blocks_further_drawing = true;
}

void Scene_Wrapped_Text::pushed()
{
	Scene::pushed();

	viewport_pivot = Vec2::zero;
	g_engine->window.set_mouse_mode( e_mouse_mode::custom );
	rc_big_text_block_01 = { 8.f, 32.f, 100.f, 100.f };
	rc_big_text_block_02 = { 120.f, 32.f, 185.f, 200.f };


	big_text_block =
		R"(Lorem ipsum dolor sit amet, consectetur adipiscing elit, )"
		R"(sed do eiusmod tempor incididunt ut labore et dolore magna )"
		R"(aliqua. Urna nunc id cursus metus. Sagittis vitae et leo duis. )"
		R"(Pretium fusce id velit ut tortor pretium viverra. Diam in arcu )"
		R"(cursus euismod. Tristique sollicitudin nibh sit amet commodo nulla )"
		R"(facilisi. Dui faucibus in ornare quam viverra orci sagittis eu. )"
		R"(Odio ut enim blandit volutpat maecenas volutpat. Risus ultricies )"
		R"(tristique nulla aliquet enim. Amet tellus cras adipiscing enim.)";

	wrapped_lines_01 = Render::wrap_string_to_width( big_text_block, rc_big_text_block_01.w );
	wrapped_lines_02 = Render::wrap_string_to_width( big_text_block, rc_big_text_block_02.w );
}

void Scene_Wrapped_Text::draw()
{
	draw_tiled_background();
	Scene::draw();
	Render::draw_world_axis();
}

void Scene_Wrapped_Text::draw_ui()
{
	Scene::draw_ui();
	draw_title( "Wrapped Text" );

	auto cb = (Scene_Wrapped_Text_UI_Callback*)get_ui_callback();

	{
		scoped_render_state;

		rc_big_text_block_01.grow( 4.f );
		rc_big_text_block_02.grow( 4.f );

		Render::state->color = make_color( 0, 0.5f );
		Render::draw_rounded_filled_rect( rc_big_text_block_01, 4.f );
		Render::state->color = make_color( 0, 0.75f );
		Render::draw_rounded_rect( rc_big_text_block_01, 4.f );

		Render::state->color = make_color( 0, 0.5f );
		Render::draw_rounded_filled_rect( rc_big_text_block_02, 4.f );
		Render::state->color = make_color( 0, 0.75f );
		Render::draw_rounded_rect( rc_big_text_block_02, 4.f );

		Render::state->color = make_color( e_pal::brighter );

		std::array<e_align_t, 3> alignments = { e_align::left, e_align::hcenter, e_align::right };
		Render::state->align = alignments[ cb->radio_alignment_data.int_value() ];

		rc_big_text_block_01.shrink( 4.f );
		rc_big_text_block_02.shrink( 4.f );

		Render::state->z += zdepth_nudge;
		Render::draw_string( wrapped_lines_01, rc_big_text_block_01 );
		Render::draw_string( wrapped_lines_02, rc_big_text_block_02 );
	}

	Rect rc_panel = { 8.f, 150.f, 100.f, 56.f };
	g_ui->layout_init( rc_panel );

	g_ui
		->panel_control()
		->set_rect( rc_panel )
		->adjust_layout_to_client_area()
		->done();

	g_ui
		->radio_control( H( "rb_align_left" ) )
		->set_text( "Left" )
		->set_idx( 0 )
		->done();

	g_ui
		->radio_control( H( "rb_align_center" ) )
		->set_text( "Center" )
		->set_idx( 1 )
		->done();


	g_ui
		->radio_control( H( "rb_align_right" ) )
		->set_text( "Right" )
		->set_idx( 2 )
		->done();
}
