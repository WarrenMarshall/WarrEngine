
namespace war
{

// callback objects for the UI system are a catch all for telling
// the system what we want to do with events, or which graphics to use,
// or what the state of data is.
//
// this is how the UI system talks to the outside world.

struct UI_Callback
{
	virtual ~UI_Callback() = default;

	UI_Control_Data default_data;

	virtual UI_Control_Data* get_data( hash tag );

	virtual Texture_Asset* get_texture_for_checkbox( hash tag );
	virtual Texture_Asset* get_texture_for_radio( hash tag );

	virtual void on_control_left_clicked( hash tag, const UI_Result& result );
	virtual void on_motion( hash tag, const UI_Result& result );

	virtual size_t get_item_count( hash tag );
	virtual std::string_view get_item_for_idx( hash tag, int32_t idx );

	virtual void on_value_changed( hash tag );
	virtual bool validate_value_change( hash tag, UI_Control_Data* old_value, UI_Control_Data* new_value );

	virtual float_t get_control_padding();
	virtual Vec2 get_control_margin( e_ui_control_type_t control_type );

	virtual bool on_input_motion( const Input_Event* evt );
	bool handle_editing_key( const Input_Event* evt );
	virtual bool on_input_pressed( const Input_Event* evt );
	virtual bool on_input_held( const Input_Event* evt );
	virtual bool on_input_released( const Input_Event* evt );
	virtual bool on_input_key( const Input_Event* evt );
};

}
