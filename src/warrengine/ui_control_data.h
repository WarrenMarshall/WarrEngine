
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

struct UI_Control_Data
{
	virtual ~UI_Control_Data() = default;

	virtual bool is_expanded();
	virtual void set_expanded( bool expanded );

	[[nodiscard]] virtual std::string string_value();
	virtual void set_string_value( std::string value );

	[[nodiscard]] virtual bool bool_value();
	virtual void set_bool_value( bool value );

	[[nodiscard]] virtual float float_value();
	virtual void set_float_value( float value );

	[[nodiscard]] float float_value_internal();
	void set_float_value_internal( float value );

	[[nodiscard]] virtual int int_value();
	virtual void set_int_value( int value );

private:
	std::string _string_value;
	bool _bool_value = false;
	int _int_value = 0;
	float _float_value = 0.f;
};

// ----------------------------------------------------------------------------
// #DOP

struct UI_Slider_Control_Data final : UI_Control_Data
{
	[[nodiscard]] virtual float float_value() override;
	virtual void set_float_value( float value ) override;

	Range<float> slider_range = { 0.f, 1.f };
};

// ----------------------------------------------------------------------------

struct UI_Text_Control_Data final : UI_Control_Data
{
	std::string valid_char_list;
	size_t max_length = 0;
	size_t caret_pos = 0;
};

// ----------------------------------------------------------------------------

struct UI_Progress_Control_Data final : UI_Control_Data
{
	bool draw_percentage_as_text : 1 = true;
};

// ----------------------------------------------------------------------------

struct UI_Dropdown_Control_Data final : UI_Control_Data
{
	[[nodiscard]] virtual bool is_expanded() override;
	virtual void set_expanded( bool expanded ) override;

private:
	bool _expanded = false;
};
}
