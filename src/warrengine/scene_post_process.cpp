
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void update_shader_uniforms( scene_post_process_ui_callback* cb )
{
	g_engine->render_api.set_uniform( "ub_vignette", cb->ub_vignette.bool_value() );

	g_engine->render_api.set_uniform( "u_vignette_size", cb->u_vignette_size.float_value() );
	g_engine->render_api.set_uniform( "u_vignette_smoothness", cb->u_vignette_smoothness.float_value() );
	g_engine->render_api.set_uniform( "u_vignette_rounding", cb->u_vignette_rounding.float_value() );

	g_engine->render_api.set_uniform( "ub_sepia", cb->ub_sepia.bool_value() );
	g_engine->render_api.set_uniform( "ub_greyscale", cb->ub_greyscale.bool_value() );
	g_engine->render_api.set_uniform( "ub_invert", cb->ub_invert.bool_value() );
	g_engine->render_api.set_uniform( "ub_crt_tint", cb->ub_crt_tint.bool_value() );
	g_engine->render_api.set_uniform( "u_crt_tint_scaling_factor", cb->u_crt_tint_scaling.float_value() );
	g_engine->render_api.set_uniform( "u_crt_tint_intensity", cb->u_crt_tint_intensity.float_value() );
	g_engine->render_api.set_uniform( "ub_film_grain", cb->ub_film_grain.bool_value() );
	g_engine->render_api.set_uniform( "u_film_grain_intensity", cb->u_film_grain_intensity.float_value() );
	g_engine->render_api.set_uniform( "ub_chromatic_abberation", cb->ub_chromatic_abberation.bool_value() );
	g_engine->render_api.set_uniform( "u_chromatic_abberation_amount", cb->u_chromatic_abberation_amount.float_value() );
	g_engine->render_api.set_uniform( "ub_crt_scanlines", cb->ub_crt_scanlines.bool_value() );
	g_engine->render_api.set_uniform( "u_crt_scanlines_intensity", cb->u_crt_scanlines_intensity.float_value() );
	g_engine->render_api.set_uniform( "ub_desaturation", cb->ub_desaturation.bool_value() );
	g_engine->render_api.set_uniform( "u_desaturation_amount", cb->u_desaturation_amount.float_value() );
	g_engine->render_api.set_uniform( "ub_crt_warp", cb->ub_crt_warp.bool_value() );
	g_engine->render_api.set_uniform( "u_crt_warp_bend", cb->u_crt_warp_bend.float_value() );
	g_engine->render_api.set_uniform( "ub_pixelate", cb->ub_pixelate.bool_value() );
	g_engine->render_api.set_uniform( "u_pixelate_factor", cb->u_pixelate_factor.float_value() );
}

// ----------------------------------------------------------------------------

scene_post_process_ui_callback::scene_post_process_ui_callback()
{
	u_vignette_size.slider_range = range( 1.7f, 2.5f );
	u_vignette_smoothness.slider_range = range( 0.5f, 1.5f );
	u_vignette_rounding.slider_range = range( 8.f, 16.f );
	u_crt_tint_scaling.slider_range = range( 0.25f, 4.f );
	u_film_grain_intensity.slider_range = range( 0.f, 2.f );
	u_chromatic_abberation_amount.slider_range = range( 0.0f, 0.01f );
	u_crt_scanlines_intensity.slider_range = range( 0.0f, 0.1f );
	u_crt_warp_bend.slider_range = range( 2.0f, 12.f );
	u_pixelate_factor.slider_range = range( 1.0f, 32.f );

	ub_vignette.set_bool_value( g_engine->render_api.get_uniform_float( "ub_vignette" ) );

	u_vignette_size.set_float_value( g_engine->render_api.get_uniform_float( "u_vignette_size" ) );
	u_vignette_smoothness.set_float_value( g_engine->render_api.get_uniform_float( "u_vignette_smoothness" ) );
	u_vignette_rounding.set_float_value( g_engine->render_api.get_uniform_float( "u_vignette_rounding" ) );

	ub_sepia.set_bool_value( g_engine->render_api.get_uniform_float( "ub_sepia" ) );
	ub_greyscale.set_bool_value( g_engine->render_api.get_uniform_float( "ub_greyscale" ) );
	ub_invert.set_bool_value( g_engine->render_api.get_uniform_float( "ub_invert" ) );
	ub_crt_tint.set_bool_value( g_engine->render_api.get_uniform_float( "ub_crt_tint" ) );
	u_crt_tint_scaling.set_float_value( g_engine->render_api.get_uniform_float( "u_crt_tint_scaling_factor" ) );
	u_crt_tint_intensity.set_float_value( g_engine->render_api.get_uniform_float( "u_crt_tint_intensity" ) );
	ub_film_grain.set_bool_value( g_engine->render_api.get_uniform_float( "ub_film_grain" ) );
	u_film_grain_intensity.set_float_value( g_engine->render_api.get_uniform_float( "u_film_grain_intensity" ) );
	ub_chromatic_abberation.set_bool_value( g_engine->render_api.get_uniform_float( "ub_chromatic_abberation" ) );
	u_chromatic_abberation_amount.set_float_value( g_engine->render_api.get_uniform_float( "u_chromatic_abberation_amount" ) );
	ub_crt_scanlines.set_bool_value( g_engine->render_api.get_uniform_float( "ub_crt_scanlines" ) );
	u_crt_scanlines_intensity.set_float_value( g_engine->render_api.get_uniform_float( "u_crt_scanlines_intensity" ) );
	ub_desaturation.set_bool_value( g_engine->render_api.get_uniform_float( "ub_desaturation" ) );
	u_desaturation_amount.set_float_value( g_engine->render_api.get_uniform_float( "u_desaturation_amount" ) );
	ub_crt_warp.set_bool_value( g_engine->render_api.get_uniform_float( "ub_crt_warp" ) );
	u_crt_warp_bend.set_float_value( g_engine->render_api.get_uniform_float( "u_crt_warp_bend" ) );
	ub_pixelate.set_bool_value( g_engine->render_api.get_uniform_float( "ub_pixelate" ) );
	u_pixelate_factor.set_float_value( g_engine->render_api.get_uniform_float( "u_pixelate_factor" ) );
}

// ----------------------------------------------------------------------------

ui_control_data* scene_post_process_ui_callback::get_data( hash tag )
{
	switch( tag )
	{
		case H( "check_vignette" ):						return &ub_vignette;
		case H( "slider_vignette_size" ):				return &u_vignette_size;
		case H( "slider_vignette_smooth" ):				return &u_vignette_smoothness;
		case H( "slider_vignette_round" ):				return &u_vignette_rounding;
		case H( "check_sepia" ):						return &ub_sepia;
		case H( "check_greyscale" ):					return &ub_greyscale;
		case H( "check_invert" ):						return &ub_invert;
		case H( "check_crt_tint" ):						return &ub_crt_tint;
		case H( "slider_crt_tint_scaling" ):			return &u_crt_tint_scaling;
		case H( "slider_crt_tint_intensity" ):			return &u_crt_tint_intensity;
		case H( "check_film_grain" ):					return &ub_film_grain;
		case H( "slider_film_grain_intensity" ):		return &u_film_grain_intensity;
		case H( "check_chromatic_abberation" ):			return &ub_chromatic_abberation;
		case H( "slider_chromatic_abberation_amount" ):	return &u_chromatic_abberation_amount;
		case H( "check_crt_scanlines" ):				return &ub_crt_scanlines;
		case H( "slider_crt_scanlines_intensity" ):		return &u_crt_scanlines_intensity;
		case H( "check_desaturation" ):					return &ub_desaturation;
		case H( "slider_desaturation_amount" ):			return &u_desaturation_amount;
		case H( "check_crt_warp" ):						return &ub_crt_warp;
		case H( "slider_crt_warp_bend" ):				return &u_crt_warp_bend;
		case H( "check_pixelate" ):						return &ub_pixelate;
		case H( "slider_pixelate_factor" ):				return &u_pixelate_factor;
	}

	return ui_callback::get_data( tag );
}

void scene_post_process_ui_callback::on_value_changed( hash tag )
{
	update_shader_uniforms( this );
}

bool scene_post_process_ui_callback::on_input_pressed( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::key_f4:
		case input_id::key_esc:
		{
			g_engine->scenes.pop();
			return true;
		}
	}

	return false;
}

// ----------------------------------------------------------------------------

scene_post_process::scene_post_process()
{
	ui_callback = std::make_unique<scene_post_process_ui_callback>();

	tex_color_test = g_engine->find_asset<texture_asset>( "tex_color_test" );

	draws_completely_solid = false;
}

void scene_post_process::pushed()
{
	viewport_pivot = vec2::zero;
	g_engine->window.set_mouse_mode( mouse_mode::custom );
}

void scene_post_process::draw_ui()
{
	auto cb = (scene_post_process_ui_callback*)get_ui_callback();

	if( background_alpha > 0.f )
	{
		scoped_render_state;

		render::state->color = make_color( color::black, background_alpha );
		render::draw_quad( g_engine->tex_white, { 0.f, 0.f, ui_w, ui_h } );
	}

	scene::draw_ui();

	auto rc_layout = rect( 8.f, 30.f, ui_hw, ui_h );

	g_ui->layout_init( rc_layout );

	// ----------------------------------------------------------------------------
	// vignette

	g_ui->check_control( H( "check_vignette" ) )
		->set_text( "Vignette" )
		->end();

	if( cb->ub_vignette.bool_value() )
	{
		float height = ui_label_control::get_default_height();

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->cut_left( ui_spacer_control::get_default_width() * 3.f );
			g_ui->label_control()->set_text( "Size:" )->cut_left( 48.f )->end();
			g_ui->slider_control( H( "slider_vignette_size" ) )->cut_left( 64.f )->end();
			g_ui->spacer_control()->cut_left()->end();

			g_ui->label_control()
				->set_text( std::format( "{:.2f}", cb->u_vignette_size.float_value() ) )
				->cut_left( 32.f )
				->end();
		}

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->cut_left( ui_spacer_control::get_default_width() * 3.f );
			g_ui->label_control()->set_text( "Smooth:" )->cut_left( 48.f )->end();
			g_ui->slider_control( H( "slider_vignette_smooth" ) )->cut_left( 64.f )->end();
			g_ui->spacer_control()->cut_left()->end();

			range vignette_smooth_range( 0.5f, 1.5f );

			g_ui->label_control()
				->set_text( std::format( "{:.2f}", cb->u_vignette_smoothness.float_value() ) )
				->cut_left( 32.f )
				->end();
		}

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->cut_left( ui_spacer_control::get_default_width() * 3.f );
			g_ui->label_control()->set_text( "Round:" )->cut_left( 48.f )->end();
			g_ui->slider_control( H( "slider_vignette_round" ) )->cut_left( 64.f )->end();
			g_ui->spacer_control()->cut_left()->end();

			range vignette_round_range( 8.f, 16.f );

			g_ui->label_control()
				->set_text( std::format( "{:.2f}", cb->u_vignette_rounding.float_value() ) )
				->cut_left( 32.f )
				->end();
		}
	}

	// ----------------------------------------------------------------------------
	// chromatic_abberation

	g_ui->check_control( H( "check_chromatic_abberation" ) )
		->set_text( "Chromatic Abberation" )
		->end();

	if( cb->ub_chromatic_abberation.bool_value() )
	{
		float height = ui_label_control::get_default_height();

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->cut_left( ui_spacer_control::get_default_width() * 3.f );
			g_ui->label_control()->set_text( "Intens:" )->cut_left( 48.f )->end();
			g_ui->slider_control( H( "slider_chromatic_abberation_amount" ) )->cut_left( 48.f )->end();
			g_ui->spacer_control()->cut_left()->end();

			g_ui->label_control()
				->set_text( std::format( "{:.4f}", cb->u_chromatic_abberation_amount.float_value() ) )
				->cut_left( 32.f )
				->end();
		}
	}

	// ----------------------------------------------------------------------------
	// desaturation

	g_ui->check_control( H( "check_desaturation" ) )
		->set_text( "Desaturation" )
		->end();

	if( cb->ub_desaturation.bool_value() )
	{
		float height = ui_label_control::get_default_height();

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->cut_left( ui_spacer_control::get_default_width() * 3.f );
			g_ui->label_control()->set_text( "Amount:" )->cut_left( 48.f )->end();
			g_ui->slider_control( H( "slider_desaturation_amount" ) )->cut_left( 64.f )->end();
			g_ui->spacer_control()->cut_left()->end();

			g_ui->label_control()
				->set_text( std::format( "{:.2f}", cb->u_desaturation_amount.float_value() ) )
				->cut_left( 32.f )
				->end();
		}
	}

	// ----------------------------------------------------------------------------
	// crt_warp

	g_ui->check_control( H( "check_crt_warp" ) )
		->set_text( "CRT Warp" )
		->end();

	if( cb->ub_crt_warp.bool_value() )
	{
		float height = ui_label_control::get_default_height();

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->cut_left( ui_spacer_control::get_default_width() * 3.f );
			g_ui->label_control()->set_text( "Bend:" )->cut_left( 48.f )->end();
			g_ui->slider_control( H( "slider_crt_warp_bend" ) )->cut_left( 64.f )->end();
			g_ui->spacer_control()->cut_left()->end();

			g_ui->label_control()
				->set_text( std::format( "{:.2f}", cb->u_crt_warp_bend.float_value() ) )
				->cut_left( 32.f )
				->end();
		}
	}

	// ----------------------------------------------------------------------------
	// pixelate

	g_ui->check_control( H( "check_pixelate" ) )
		->set_text( "Pixelate" )
		->end();

	if( cb->ub_pixelate.bool_value() )
	{
		float height = ui_label_control::get_default_height();

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->cut_left( ui_spacer_control::get_default_width() * 3.f );
			g_ui->label_control()->set_text( "Factor:" )->cut_left( 48.f )->end();
			g_ui->slider_control( H( "slider_pixelate_factor" ) )->cut_left( 64.f )->end();
			g_ui->spacer_control()->cut_left()->end();

			g_ui->label_control()
				->set_text( std::format( "{:.2f}", cb->u_pixelate_factor.float_value() ) )
				->cut_left( 32.f )
				->end();
		}
	}

	rc_layout = rect( ui_hw + 8.0f, 30.f, ui_hw, ui_h );

	g_ui->layout_init( rc_layout );

	// ----------------------------------------------------------------------------
	// sepia

	g_ui->check_control( H( "check_sepia" ) )
		->set_text( "Sepia Tone" )
		->end();

	// ----------------------------------------------------------------------------
	// greyscale

	g_ui->check_control( H( "check_greyscale" ) )
		->set_text( "Greyscale" )
		->end();

	// ----------------------------------------------------------------------------
	// invert

	g_ui->check_control( H( "check_invert" ) )
		->set_text( "Invert" )
		->end();

	// ----------------------------------------------------------------------------
	// crt_tint

	g_ui->check_control( H( "check_crt_tint" ) )
		->set_text( "CRT Tint Lines" )
		->end();

	if( cb->ub_crt_tint.bool_value() )
	{
		float height = ui_label_control::get_default_height();

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->cut_left( ui_spacer_control::get_default_width() * 3.f );
			g_ui->label_control()->set_text( "Scale:" )->cut_left( 48.f )->end();
			g_ui->slider_control( H( "slider_crt_tint_scaling" ) )->cut_left( 64.f )->end();
			g_ui->spacer_control()->cut_left()->end();

			g_ui->label_control()
				->set_text( std::format( "{:.2f}", cb->u_crt_tint_scaling.float_value() ) )
				->cut_left( 32.f )
				->end();
		}

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->cut_left( ui_spacer_control::get_default_width() * 3.f );
			g_ui->label_control()->set_text( "Intens:" )->cut_left( 48.f )->end();
			g_ui->slider_control( H( "slider_crt_tint_intensity" ) )->cut_left( 64.f )->end();
			g_ui->spacer_control()->cut_left()->end();

			g_ui->label_control()
				->set_text( std::format( "{:.2f}", cb->u_crt_tint_intensity.float_value() ) )
				->cut_left( 32.f )
				->end();
		}
	}

	// ----------------------------------------------------------------------------
	// crt_scanlines

	g_ui->check_control( H( "check_crt_scanlines" ) )
		->set_text( "CRT Scanlines" )
		->end();

	if( cb->ub_crt_scanlines.bool_value() )
	{
		float height = ui_label_control::get_default_height();

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->cut_left( ui_spacer_control::get_default_width() * 3.f );
			g_ui->label_control()->set_text( "Intens:" )->cut_left( 48.f )->end();
			g_ui->slider_control( H( "slider_crt_scanlines_intensity" ) )->cut_left( 48.f )->end();
			g_ui->spacer_control()->cut_left()->end();

			g_ui->label_control()
				->set_text( std::format( "{:.4f}", cb->u_crt_scanlines_intensity.float_value() ) )
				->cut_left( 32.f )
				->end();
		}
	}

	// ----------------------------------------------------------------------------
	// film_grain

	g_ui->check_control( H( "check_film_grain" ) )
		->set_text( "Film Grain" )
		->end();

	if( cb->ub_film_grain.bool_value() )
	{
		float height = ui_label_control::get_default_height();

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->cut_left( ui_spacer_control::get_default_width() * 3.f );
			g_ui->label_control()->set_text( "Intens:" )->cut_left( 48.f )->end();
			g_ui->slider_control( H( "slider_film_grain_intensity" ) )->cut_left( 64.f )->end();
			g_ui->spacer_control()->cut_left()->end();

			g_ui->label_control()
				->set_text( std::format( "{:.2f}", cb->u_film_grain_intensity.float_value() ) )
				->cut_left( 32.f )
				->end();
		}
	}

	// ----------------------------------------------------------------------------
	// test image

	{
		scoped_render_state;
		render::state->scale = 0.65f;

		g_ui->image_control()
			->set_image( tex_color_test )
			->cut_top( tex_color_test->height() * render::state->scale.y )
			->end();
	}

}

bool scene_post_process::on_input_motion( const input_event* evt )
{
	if( evt->input_id == input_id::mouse_wheel )
	{
		background_alpha += evt->delta.y * 0.05f;
		background_alpha = glm::clamp( background_alpha, 0.f, 1.f );

		return true;
	}

	return false;
}

}
