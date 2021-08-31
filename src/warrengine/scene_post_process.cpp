
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void update_shader_uniforms( Scene_Post_Process_UI_Callback* cb )
{
	g_engine->opengl_mgr.set_uniform_bool( "ub_vignette", cb->ub_vignette.bool_value() );

	g_engine->opengl_mgr.set_uniform_float( "u_vignette_size", cb->u_vignette_size.float_value() );
	g_engine->opengl_mgr.set_uniform_float( "u_vignette_smoothness", cb->u_vignette_smoothness.float_value() );
	g_engine->opengl_mgr.set_uniform_float( "u_vignette_rounding", cb->u_vignette_rounding.float_value() );

	g_engine->opengl_mgr.set_uniform_bool( "ub_crt_tint", cb->ub_crt_tint.bool_value() );
	g_engine->opengl_mgr.set_uniform_float( "u_crt_tint_scaling_factor", cb->u_crt_tint_scaling.float_value() );
	g_engine->opengl_mgr.set_uniform_float( "u_crt_tint_intensity", cb->u_crt_tint_intensity.float_value() );
	g_engine->opengl_mgr.set_uniform_bool( "ub_film_grain", cb->ub_film_grain.bool_value() );
	g_engine->opengl_mgr.set_uniform_float( "u_film_grain_intensity", cb->u_film_grain_intensity.float_value() );
	g_engine->opengl_mgr.set_uniform_bool( "ub_chromatic_aberration", cb->ub_chromatic_aberration.bool_value() );
	g_engine->opengl_mgr.set_uniform_float( "u_chromatic_aberration_amount", cb->u_chromatic_aberration_amount.float_value() );
	g_engine->opengl_mgr.set_uniform_bool( "ub_crt_scanlines", cb->ub_crt_scanlines.bool_value() );
	g_engine->opengl_mgr.set_uniform_float( "u_crt_scanlines_intensity", cb->u_crt_scanlines_intensity.float_value() );
	g_engine->opengl_mgr.set_uniform_bool( "ub_desaturation", cb->ub_desaturation.bool_value() );
	g_engine->opengl_mgr.set_uniform_float( "u_desaturation_amount", cb->u_desaturation_amount.float_value() );
	g_engine->opengl_mgr.set_uniform_bool( "ub_crt_warp", cb->ub_crt_warp.bool_value() );
	g_engine->opengl_mgr.set_uniform_float( "u_crt_warp_bend", cb->u_crt_warp_bend.float_value() );
	g_engine->opengl_mgr.set_uniform_float( "u_pixelate_factor", cb->u_pixelate_factor.float_value() );
}

// ----------------------------------------------------------------------------

Scene_Post_Process_UI_Callback::Scene_Post_Process_UI_Callback()
{
	u_vignette_size.slider.range = Range( 1.7f, 2.5f );
	u_vignette_smoothness.slider.range = Range( 0.5f, 1.5f );
	u_vignette_rounding.slider.range = Range( 8.f, 16.f );
	u_crt_tint_scaling.slider.range = Range( 0.25f, 4.f );
	u_film_grain_intensity.slider.range = Range( 0.f, 2.f );
	u_chromatic_aberration_amount.slider.range = Range( 0.f, 0.01f );
	u_crt_scanlines_intensity.slider.range = Range( 0.f, 0.1f );
	u_crt_warp_bend.slider.range = Range( 2.f, 12.f );
	u_pixelate_factor.slider.range = Range( 0.f, 32.f );

	ub_vignette.set_bool_value( g_engine->opengl_mgr.get_uniform_float( "ub_vignette" ) );

	u_vignette_size.set_float_value( g_engine->opengl_mgr.get_uniform_float( "u_vignette_size" ) );
	u_vignette_smoothness.set_float_value( g_engine->opengl_mgr.get_uniform_float( "u_vignette_smoothness" ) );
	u_vignette_rounding.set_float_value( g_engine->opengl_mgr.get_uniform_float( "u_vignette_rounding" ) );

	ub_crt_tint.set_bool_value( g_engine->opengl_mgr.get_uniform_float( "ub_crt_tint" ) );
	u_crt_tint_scaling.set_float_value( g_engine->opengl_mgr.get_uniform_float( "u_crt_tint_scaling_factor" ) );
	u_crt_tint_intensity.set_float_value( g_engine->opengl_mgr.get_uniform_float( "u_crt_tint_intensity" ) );
	ub_film_grain.set_bool_value( g_engine->opengl_mgr.get_uniform_float( "ub_film_grain" ) );
	u_film_grain_intensity.set_float_value( g_engine->opengl_mgr.get_uniform_float( "u_film_grain_intensity" ) );
	ub_chromatic_aberration.set_bool_value( g_engine->opengl_mgr.get_uniform_float( "ub_chromatic_aberration" ) );
	u_chromatic_aberration_amount.set_float_value( g_engine->opengl_mgr.get_uniform_float( "u_chromatic_aberration_amount" ) );
	ub_crt_scanlines.set_bool_value( g_engine->opengl_mgr.get_uniform_float( "ub_crt_scanlines" ) );
	u_crt_scanlines_intensity.set_float_value( g_engine->opengl_mgr.get_uniform_float( "u_crt_scanlines_intensity" ) );
	ub_desaturation.set_bool_value( g_engine->opengl_mgr.get_uniform_float( "ub_desaturation" ) );
	u_desaturation_amount.set_float_value( g_engine->opengl_mgr.get_uniform_float( "u_desaturation_amount" ) );
	ub_crt_warp.set_bool_value( g_engine->opengl_mgr.get_uniform_float( "ub_crt_warp" ) );
	u_crt_warp_bend.set_float_value( g_engine->opengl_mgr.get_uniform_float( "u_crt_warp_bend" ) );
	u_pixelate_factor.set_float_value( g_engine->opengl_mgr.get_uniform_float( "u_pixelate_factor" ) );
}

// ----------------------------------------------------------------------------

UI_Control_Data* Scene_Post_Process_UI_Callback::get_data( hash tag )
{
	switch( tag )
	{
		case H( "check_vignette" ):						return &ub_vignette;
		case H( "slider_vignette_size" ):				return &u_vignette_size;
		case H( "slider_vignette_smooth" ):				return &u_vignette_smoothness;
		case H( "slider_vignette_round" ):				return &u_vignette_rounding;
		case H( "check_crt_tint" ):						return &ub_crt_tint;
		case H( "slider_crt_tint_scaling" ):			return &u_crt_tint_scaling;
		case H( "slider_crt_tint_intensity" ):			return &u_crt_tint_intensity;
		case H( "check_film_grain" ):					return &ub_film_grain;
		case H( "slider_film_grain_intensity" ):		return &u_film_grain_intensity;
		case H( "check_chromatic_aberration" ):			return &ub_chromatic_aberration;
		case H( "slider_chromatic_aberration_amount" ):	return &u_chromatic_aberration_amount;
		case H( "check_crt_scanlines" ):				return &ub_crt_scanlines;
		case H( "slider_crt_scanlines_intensity" ):		return &u_crt_scanlines_intensity;
		case H( "check_desaturation" ):					return &ub_desaturation;
		case H( "slider_desaturation_amount" ):			return &u_desaturation_amount;
		case H( "check_crt_warp" ):						return &ub_crt_warp;
		case H( "slider_crt_warp_bend" ):				return &u_crt_warp_bend;
		case H( "slider_pixelate_factor" ):				return &u_pixelate_factor;
	}

	return UI_Callback::get_data( tag );
}

void Scene_Post_Process_UI_Callback::on_value_changed( hash tag )
{
	update_shader_uniforms( this );
}

bool Scene_Post_Process_UI_Callback::on_input_pressed( const Input_Event* evt )
{
	switch( evt->input_id )
	{
		case e_input_id::key_f4:
		case e_input_id::key_esc:
		{
			g_engine->scene_mgr.pop();
			return true;
		}
	}

	return false;
}

// ----------------------------------------------------------------------------

Scene_Post_Process::Scene_Post_Process()
{
	ui_callback = std::make_unique<Scene_Post_Process_UI_Callback>();

	flags.blocks_further_drawing = false;
}

void Scene_Post_Process::pushed()
{
	Scene::pushed();

	viewport_pivot = Vec2::zero;
	g_engine->window.set_mouse_mode( e_mouse_mode::custom );
}

void Scene_Post_Process::draw_ui()
{
	auto cb = (Scene_Post_Process_UI_Callback*)get_ui_callback();

	if( background_alpha > 0.f )
	{
		scoped_render_state;

		Render::state->color = make_color( Color::black, background_alpha );
		Render::draw_quad( g_engine->tex_white, { 0.f, 0.f, ui_w, ui_h } );
	}

	Scene::draw_ui();

	auto rc_layout = Rect( 8.f, 30.f, ui_w, ui_h );
	g_ui->layout_init( rc_layout );

	// ----------------------------------------------------------------------------
	// default lut

	g_ui->image_control()
		->set_image( g_engine->tex_default_lut )
		->cut_top( g_engine->tex_default_lut->height() )
		->set_size( { viewport_w, g_engine->tex_default_lut->height() } )
		->done();

	rc_layout = Rect( 8.f, 30.f + g_engine->tex_default_lut->height(), ui_hw, ui_h );
	g_ui->layout_init( rc_layout );

	// ----------------------------------------------------------------------------
	// vignette

	g_ui->check_control( H( "check_vignette" ) )
		->set_text( "Vignette" )
		->done();

	if( cb->ub_vignette.bool_value() )
	{
		float height = UI_Label_Control::get_default_height();

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->cut_left( UI_Spacer_Control::get_default_width() * 3.f );
			g_ui->label_control()->set_text( "Size:" )->cut_left( 48.f )->done();
			g_ui->slider_control( H( "slider_vignette_size" ) )->cut_left( 64.f )->done();
			g_ui->spacer_control()->cut_left()->done();

			g_ui->label_control()
				->set_text( std::format( "{:.2f}", cb->u_vignette_size.float_value() ) )
				->cut_left( 32.f )
				->done();
		}

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->cut_left( UI_Spacer_Control::get_default_width() * 3.f );
			g_ui->label_control()->set_text( "Smooth:" )->cut_left( 48.f )->done();
			g_ui->slider_control( H( "slider_vignette_smooth" ) )->cut_left( 64.f )->done();
			g_ui->spacer_control()->cut_left()->done();

			Range vignette_smooth_range( 0.5f, 1.5f );

			g_ui->label_control()
				->set_text( std::format( "{:.2f}", cb->u_vignette_smoothness.float_value() ) )
				->cut_left( 32.f )
				->done();
		}

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->cut_left( UI_Spacer_Control::get_default_width() * 3.f );
			g_ui->label_control()->set_text( "Round:" )->cut_left( 48.f )->done();
			g_ui->slider_control( H( "slider_vignette_round" ) )->cut_left( 64.f )->done();
			g_ui->spacer_control()->cut_left()->done();

			Range vignette_round_range( 8.f, 16.f );

			g_ui->label_control()
				->set_text( std::format( "{:.2f}", cb->u_vignette_rounding.float_value() ) )
				->cut_left( 32.f )
				->done();
		}
	}

	// ----------------------------------------------------------------------------
	// chromatic_aberration

	g_ui->check_control( H( "check_chromatic_aberration" ) )
		->set_text( "Chromatic Aberration" )
		->done();

	if( cb->ub_chromatic_aberration.bool_value() )
	{
		float height = UI_Label_Control::get_default_height();

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->cut_left( UI_Spacer_Control::get_default_width() * 3.f );
			g_ui->label_control()->set_text( "Intens:" )->cut_left( 48.f )->done();
			g_ui->slider_control( H( "slider_chromatic_aberration_amount" ) )->cut_left( 48.f )->done();
			g_ui->spacer_control()->cut_left()->done();

			g_ui->label_control()
				->set_text( std::format( "{:.4f}", cb->u_chromatic_aberration_amount.float_value() ) )
				->cut_left( 32.f )
				->done();
		}
	}

	// ----------------------------------------------------------------------------
	// desaturation

	g_ui->check_control( H( "check_desaturation" ) )
		->set_text( "Desaturation" )
		->done();

	if( cb->ub_desaturation.bool_value() )
	{
		float height = UI_Label_Control::get_default_height();

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->cut_left( UI_Spacer_Control::get_default_width() * 3.f );
			g_ui->label_control()->set_text( "Amount:" )->cut_left( 48.f )->done();
			g_ui->slider_control( H( "slider_desaturation_amount" ) )->cut_left( 64.f )->done();
			g_ui->spacer_control()->cut_left()->done();

			g_ui->label_control()
				->set_text( std::format( "{:.2f}", cb->u_desaturation_amount.float_value() ) )
				->cut_left( 32.f )
				->done();
		}
	}

	// ----------------------------------------------------------------------------
	// crt_warp

	g_ui->check_control( H( "check_crt_warp" ) )
		->set_text( "CRT Warp" )
		->done();

	if( cb->ub_crt_warp.bool_value() )
	{
		float height = UI_Label_Control::get_default_height();

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->cut_left( UI_Spacer_Control::get_default_width() * 3.f );
			g_ui->label_control()->set_text( "Bend:" )->cut_left( 48.f )->done();
			g_ui->slider_control( H( "slider_crt_warp_bend" ) )->cut_left( 64.f )->done();
			g_ui->spacer_control()->cut_left()->done();

			g_ui->label_control()
				->set_text( std::format( "{:.2f}", cb->u_crt_warp_bend.float_value() ) )
				->cut_left( 32.f )
				->done();
		}
	}

	// ----------------------------------------------------------------------------
	// pixelate

	{
		float height = UI_Label_Control::get_default_height();

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->label_control()->set_text( "Pixelate:" )
				->cut_left( 56.f )
				->set_text_color( make_color( e_pal::brightest ) )
				->done();
			g_ui->slider_control( H( "slider_pixelate_factor" ) )->cut_left( 64.f )->done();
			g_ui->spacer_control()->cut_left()->done();

			g_ui->label_control()
				->set_text( std::format( "{:.2f}", cb->u_pixelate_factor.float_value() ) )
				->set_text_color( make_color( e_pal::brightest ) )
				->cut_left( 32.f )
				->done();
		}
	}

	// move to right side of screen

	rc_layout = Rect( ui_hw + 8.f, 30.f + g_engine->tex_default_lut->height(), ui_hw, ui_h );
	g_ui->layout_init( rc_layout );

	// ----------------------------------------------------------------------------
	// crt_tint

	g_ui->check_control( H( "check_crt_tint" ) )
		->set_text( "CRT Tint Lines" )
		->done();

	if( cb->ub_crt_tint.bool_value() )
	{
		float height = UI_Label_Control::get_default_height();

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->cut_left( UI_Spacer_Control::get_default_width() * 3.f );
			g_ui->label_control()->set_text( "Scale:" )->cut_left( 48.f )->done();
			g_ui->slider_control( H( "slider_crt_tint_scaling" ) )->cut_left( 64.f )->done();
			g_ui->spacer_control()->cut_left()->done();

			g_ui->label_control()
				->set_text( std::format( "{:.2f}", cb->u_crt_tint_scaling.float_value() ) )
				->cut_left( 32.f )
				->done();
		}

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->cut_left( UI_Spacer_Control::get_default_width() * 3.f );
			g_ui->label_control()->set_text( "Intens:" )->cut_left( 48.f )->done();
			g_ui->slider_control( H( "slider_crt_tint_intensity" ) )->cut_left( 64.f )->done();
			g_ui->spacer_control()->cut_left()->done();

			g_ui->label_control()
				->set_text( std::format( "{:.2f}", cb->u_crt_tint_intensity.float_value() ) )
				->cut_left( 32.f )
				->done();
		}
	}

	// ----------------------------------------------------------------------------
	// crt_scanlines

	g_ui->check_control( H( "check_crt_scanlines" ) )
		->set_text( "CRT Scanlines" )
		->done();

	if( cb->ub_crt_scanlines.bool_value() )
	{
		float height = UI_Label_Control::get_default_height();

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->cut_left( UI_Spacer_Control::get_default_width() * 3.f );
			g_ui->label_control()->set_text( "Intens:" )->cut_left( 48.f )->done();
			g_ui->slider_control( H( "slider_crt_scanlines_intensity" ) )->cut_left( 48.f )->done();
			g_ui->spacer_control()->cut_left()->done();

			g_ui->label_control()
				->set_text( std::format( "{:.4f}", cb->u_crt_scanlines_intensity.float_value() ) )
				->cut_left( 32.f )
				->done();
		}
	}

	// ----------------------------------------------------------------------------
	// film_grain

	g_ui->check_control( H( "check_film_grain" ) )
		->set_text( "Film Grain" )
		->done();

	if( cb->ub_film_grain.bool_value() )
	{
		float height = UI_Label_Control::get_default_height();

		{
			scoped_ui_layout( g_ui->layout_top_ptr()->cut_top( height ) );

			g_ui->cut_left( UI_Spacer_Control::get_default_width() * 3.f );
			g_ui->label_control()->set_text( "Intens:" )->cut_left( 48.f )->done();
			g_ui->slider_control( H( "slider_film_grain_intensity" ) )->cut_left( 64.f )->done();
			g_ui->spacer_control()->cut_left()->done();

			g_ui->label_control()
				->set_text( std::format( "{:.2f}", cb->u_film_grain_intensity.float_value() ) )
				->cut_left( 32.f )
				->done();
		}
	}
}

bool Scene_Post_Process::on_input_motion( const Input_Event* evt )
{
	if( evt->input_id == e_input_id::mouse_wheel )
	{
		background_alpha += evt->delta.y * 0.05f;
		background_alpha = glm::clamp( background_alpha, 0.f, 1.f );

		return true;
	}

	return false;
}

}
