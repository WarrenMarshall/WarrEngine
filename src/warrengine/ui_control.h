
namespace war
{

struct UI_Control
{
	e_ui_control_type_t type = e_ui_control_type::none;

	hash tag;
	std::string text;
	e_align_t text_align = e_align::hcenter | e_align::vcenter;
	Color text_color = Color::white;
	Color primary_color = Color::black;
	Slide_Def_Asset* slice_def = nullptr;
	int32_t idx = -1;
	size_t total = 0;
	Texture_Asset* image = nullptr;
	float_t image_scale = 1.f;
	f_draw_control func_draw_control = nullptr;

	// expressed as a 0-1 percentage.  So 0.2 would be 5 intervals, 0.1 would be 10, etc.
	float_t interval = 0.f;

	// the ui and client rectangles
	Rect rc_ui = {};
	Rect rc_client = {};

	// is this control an "active" or "passive" control?
	bool is_active : 1 = false;

	// has the "set_pos" call been made for this control? used when
	// deciding whether or not to use default sizes.
	bool set_pos_called : 1 = false;

	// does this control do a position offset when it's made hot / clicked?
	bool uses_click_offset : 1 = true;

	// can this control retain focus across frames? if true, then this control
	// will remain "hot" until the user is done interacting with it.
	bool can_retain_focus : 1 = false;

	UI_Control( hash tag = hash_none );
	virtual ~UI_Control() = default;

	// #ui - remove all these references to rc_ui and rc_client - use the member vars we already have
	void draw_slice_def( const Rect & rc_ui, bool is_hovered, bool is_hot );
	void draw_text( const Rect & rc_client, const Color & color, bool is_hovered, bool is_hot, const std::string & text );
	void draw_texture( const Rect & rc, Texture_Asset * texture, bool is_hovered, bool is_hot );
	void draw_image( const Rect & rc, Texture_Asset * texture );

	virtual void draw( const Rect & rc_ui, const Rect & rc_client, bool is_hovered, bool is_hot );

	virtual Vec2 get_control_inner_margins();

	float_t default_width = 0.f;
	float_t default_height = 0.f;
};

// ----------------------------------------------------------------------------
// #DOP - it feels like all these subclasses could be reduced to DOP

struct UI_Panel_Control : UI_Control
{
	UI_Panel_Control( hash tag = hash_none );

	virtual void draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float_t get_default_width();
	static float_t get_default_height();
};

struct UI_Caption_Control : UI_Control
{
	UI_Caption_Control( hash tag = hash_none );

	virtual void draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float_t get_default_width();
	static float_t get_default_height();
};

struct UI_Button_Control : UI_Control
{
	UI_Button_Control( hash tag = hash_none );

	virtual void draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float_t get_default_width();
	static float_t get_default_height();
};

struct UI_Check_Control : UI_Control
{
	UI_Check_Control( hash tag = hash_none );


	virtual void draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float_t get_default_width();
	static float_t get_default_height();
};

struct UI_Divider_Control : UI_Control
{
	UI_Divider_Control( hash tag = hash_none );

	virtual void draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float_t get_default_width();
	static float_t get_default_height();
	virtual Vec2 get_control_inner_margins() override;
};

struct UI_Spacer_Control : UI_Control
{
	UI_Spacer_Control( hash tag = hash_none );

	virtual void draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float_t get_default_width();
	static float_t get_default_height();
	virtual Vec2 get_control_inner_margins() override;
};


struct UI_Image_Control : UI_Control
{
	UI_Image_Control( hash tag = hash_none );

	virtual void draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float_t get_default_width();
	static float_t get_default_height();
};

struct UI_Label_Control : UI_Control
{
	UI_Label_Control( hash tag = hash_none );

	virtual void draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float_t get_default_width();
	static float_t get_default_height();
};

struct UI_Slider_Control : UI_Control
{
	UI_Slider_Control( hash tag = hash_none );

	virtual void draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot ) override;
	void draw_slider_thumb( const Rect& rc_client );
	void draw_slider_tick_marks( const Rect& rc_ui, const Rect& rc_client );

	static float_t get_default_width();
	static float_t get_default_height();
	virtual Vec2 get_control_inner_margins() override;
};

struct UI_Text_Control : UI_Control
{
	UI_Text_Control( hash tag = hash_none );

	virtual void draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float_t get_default_width();
	static float_t get_default_height();
};

struct UI_Radio_Control : UI_Control
{
	UI_Radio_Control( hash tag = hash_none );

	virtual void draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float_t get_default_width();
	static float_t get_default_height();
};

struct UI_Progress_Control : UI_Control
{
	UI_Progress_Control( hash tag = hash_none );


	virtual void draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float_t get_default_width();
	static float_t get_default_height();
};

struct UI_List_Control : UI_Control
{
	UI_List_Control( hash tag = hash_none );

	virtual void draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float_t get_default_width();
	static float_t get_default_height();
};

struct UI_Dropdown_Control : UI_Control
{
	UI_Dropdown_Control( hash tag = hash_none );

	virtual void draw( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float_t get_default_width();
	static float_t get_default_height();
};

}
