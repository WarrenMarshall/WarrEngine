
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

ui_control_data* scene_wrapped_text_ui_callback::get_data( hash tag )
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

	return ui_callback::get_data( tag );
}

// ----------------------------------------------------------------------------

scene_wrapped_text::scene_wrapped_text()
{
	ui_callback = std::make_unique<scene_wrapped_text_ui_callback>();

	flags.blocks_further_drawing = true;
}

void scene_wrapped_text::pushed()
{
	viewport_pivot = vec2::zero;
	g_engine->window.set_mouse_mode( mouse_mode::custom );
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

	wrapped_lines_01 = render::wrap_string_to_width( big_text_block, rc_big_text_block_01.w );
	wrapped_lines_02 = render::wrap_string_to_width( big_text_block, rc_big_text_block_02.w );
}

void scene_wrapped_text::draw()
{
	draw_tiled_background();
	scene::draw();
	render::draw_world_axis();
}

void scene_wrapped_text::draw_ui()
{
	scene::draw_ui();
	draw_title( "Wrapped Text" );

	auto cb = (scene_wrapped_text_ui_callback*)get_ui_callback();

	{
		scoped_render_state;

		rc_big_text_block_01.grow( 4.f );
		rc_big_text_block_02.grow( 4.f );


		render::state->color = make_color( 0, 0.5f );
		render::draw_rounded_filled_rect( rc_big_text_block_01, 4.f );
		render::state->color = make_color( 0, 0.75f );
		render::draw_rounded_rect( rc_big_text_block_01, 4.f );

		render::state->color = make_color( 0, 0.5f );
		render::draw_rounded_filled_rect( rc_big_text_block_02, 4.f );
		render::state->color = make_color( 0, 0.75f );
		render::draw_rounded_rect( rc_big_text_block_02, 4.f );

		render::state->color = make_color( pal::brighter );

		std::array<e_align, 3> alignments = { align::left, align::hcenter, align::right };
		render::state->align = alignments[ cb->radio_alignment_data.int_value() ];

		rc_big_text_block_01.shrink( 4.f );
		rc_big_text_block_02.shrink( 4.f );

		render::state->z += zdepth_nudge;
		render::draw_string( wrapped_lines_01, rc_big_text_block_01 );
		render::draw_string( wrapped_lines_02, rc_big_text_block_02 );

	}

	rect rc_panel = { 8.f, 150.f, 100.f, 56.f };
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
