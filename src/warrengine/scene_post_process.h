
namespace war
{

// ----------------------------------------------------------------------------

struct Scene_Post_Process_UI_Callback final : UI_Callback
{
	Scene_Post_Process_UI_Callback();

	UI_Control_Data ub_vignette = UI_Control_Data( e_ui_control_type::check );
	UI_Control_Data u_vignette_size = UI_Control_Data( e_ui_control_type::slider );
	UI_Control_Data u_vignette_smoothness = UI_Control_Data( e_ui_control_type::slider );
	UI_Control_Data u_vignette_rounding = UI_Control_Data( e_ui_control_type::slider );
	UI_Control_Data ub_crt_tint = UI_Control_Data( e_ui_control_type::check );
	UI_Control_Data u_crt_tint_scaling = UI_Control_Data( e_ui_control_type::slider );
	UI_Control_Data u_crt_tint_intensity = UI_Control_Data( e_ui_control_type::slider );
	UI_Control_Data ub_film_grain = UI_Control_Data( e_ui_control_type::check );
	UI_Control_Data u_film_grain_intensity = UI_Control_Data( e_ui_control_type::slider );
	UI_Control_Data ub_chromatic_aberration = UI_Control_Data( e_ui_control_type::check );
	UI_Control_Data u_chromatic_aberration_amount = UI_Control_Data( e_ui_control_type::slider );
	UI_Control_Data ub_crt_scanlines = UI_Control_Data( e_ui_control_type::check );
	UI_Control_Data u_crt_scanlines_intensity = UI_Control_Data( e_ui_control_type::slider );
	UI_Control_Data ub_desaturation = UI_Control_Data( e_ui_control_type::check );
	UI_Control_Data u_desaturation_amount = UI_Control_Data( e_ui_control_type::slider );
	UI_Control_Data ub_crt_warp = UI_Control_Data( e_ui_control_type::check );
	UI_Control_Data u_crt_warp_bend = UI_Control_Data( e_ui_control_type::slider );
	UI_Control_Data u_pixelate_factor = UI_Control_Data( e_ui_control_type::slider );

	virtual UI_Control_Data* get_data( hash tag ) override;
	virtual void on_value_changed( hash tag ) override;
	virtual bool on_input_pressed( const Input_Event* evt ) override;
};

// ----------------------------------------------------------------------------

struct Scene_Post_Process final : Scene
{
	Scene_Post_Process_UI_Callback uic;
	float_t background_alpha = 0.5f;

	Scene_Post_Process();

	virtual void pushed() override;
	virtual void popped() override;
	virtual void draw_ui() override;
	virtual bool on_input_motion( const Input_Event* evt ) override;
};

}
