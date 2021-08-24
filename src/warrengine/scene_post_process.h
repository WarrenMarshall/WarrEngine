
namespace war
{

// ----------------------------------------------------------------------------

struct scene_post_process_ui_callback final : ui_callback
{
	scene_post_process_ui_callback();

	ui_control_data ub_vignette;
	ui_slider_control_data u_vignette_size;
	ui_slider_control_data u_vignette_smoothness;
	ui_slider_control_data u_vignette_rounding;
	ui_control_data ub_crt_tint;
	ui_slider_control_data u_crt_tint_scaling;
	ui_slider_control_data u_crt_tint_intensity;
	ui_control_data ub_film_grain;
	ui_slider_control_data u_film_grain_intensity;
	ui_control_data ub_chromatic_aberration;
	ui_slider_control_data u_chromatic_aberration_amount;
	ui_control_data ub_crt_scanlines;
	ui_slider_control_data u_crt_scanlines_intensity;
	ui_control_data ub_desaturation;
	ui_slider_control_data u_desaturation_amount;
	ui_control_data ub_crt_warp;
	ui_slider_control_data u_crt_warp_bend;
	ui_control_data ub_pixelate;
	ui_slider_control_data u_pixelate_factor;

	virtual ui_control_data* get_data( hash tag ) override;
	virtual void on_value_changed( hash tag ) override;
	virtual bool on_input_pressed( const input_event* evt ) override;
};

// ----------------------------------------------------------------------------

struct scene_post_process final : scene
{
	float background_alpha = 0.5f;

	scene_post_process();

	virtual void pushed() override;
	virtual void draw_ui() override;
	virtual bool on_input_motion( const input_event* evt ) override;
};

}
