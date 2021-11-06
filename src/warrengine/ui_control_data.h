
namespace war
{

// these data structs are for tracking the internal values of the controls. aka
// anything that is specific to the control in question.
//
// since the ui is immediate mode, we need to store specific data for each
// control so we can get back to it next frame. these are stored in the
// ui_callback class for the scene itself.
//
// i.e. the text and caret position for an edit control, the on/off state for a
// checkbox, the item idx for a list box, etc.

struct UI_Control_Data final
{
	UI_Control_Data();
	UI_Control_Data( e_ui_control_type type );
	~UI_Control_Data();	// need this to quiet the compiler

	bool is_expanded();
	void set_expanded( bool expanded );

	[[nodiscard]] std::string string_value();
	void set_string_value( std::string value );

	[[nodiscard]] bool bool_value();
	void set_bool_value( bool value );

	[[nodiscard]] f32 float_value();
	void set_float_value( f32 value );

	[[nodiscard]] f32 float_value_internal();
	void set_float_value_internal( f32 value );

	[[nodiscard]] i32 int_value();
	void set_int_value( i32 value );

	e_ui_control_type type = e_ui_control_type::none;

	bool _bool_value = false;
	i32 _int_value = 0;
	f32 _float_value = 0.f;
	std::string _string_value;

	struct
	{
		bool draw_percentage_as_text = true;
	} progress;

	struct
	{
		bool expanded = false;
	} dropdown;

	struct
	{
		Range range = Range( 0.f, 1.f );
	} slider;

	struct
	{
		size_t max_length = 0;
		size_t caret_pos = 0;
		std::string valid_char_list;
	} text;
};

}
