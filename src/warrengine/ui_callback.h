
namespace war
{

// callback objects for the UI system are a catch all for telling
// the system what we want to do with events, or which graphics to use,
// or what the state of data is.
//
// this is how the UI system talks to the outside world.

struct ui_callback
{
	virtual ~ui_callback() = default;

	ui_control_data default_data;

	virtual ui_control_data* get_data( hash tag );

	virtual texture_asset* get_texture_for_checkbox( hash tag );
	virtual texture_asset* get_texture_for_radio( hash tag );

	virtual void on_control_left_clicked( hash tag, const ui_result& result );
	virtual void on_motion( hash tag, const ui_result& result );

	virtual size_t get_item_count( hash tag );
	virtual std::string_view get_item_for_idx( hash tag, int idx );

	virtual void on_value_changed( hash tag );
	virtual bool validate_value_change( hash tag, ui_control_data* old_value, ui_control_data* new_value );

	virtual float get_control_padding();
	virtual vec2 get_control_margin( e_ui_control_type control_type );

	virtual bool on_input_motion( const input_event* evt );
	bool handle_editing_key( const input_event* evt );
	virtual bool on_input_pressed( const input_event* evt );
	virtual bool on_input_held( const input_event* evt );
	virtual bool on_input_released( const input_event* evt );
	virtual bool on_input_key( const input_event* evt );
};

}
