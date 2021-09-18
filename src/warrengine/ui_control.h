
namespace war
{

struct UI_Control final
{
	e_ui_control_type_t type = e_ui_control_type::none;

	hash tag;
	std::string text;
	e_align_t text_align = e_align::hcenter | e_align::vcenter;
	Color text_color = Color::white;
	Color primary_color = Color::black;
	Slice_Def_Asset* slice_def = nullptr;
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

	void draw_slice_def( const Rect & rc_ui, bool is_hovered, bool is_hot );
	void draw_text( const Rect & rc_client, const Color & color, bool is_hovered, bool is_hot, const std::string & text );
	void draw_texture( const Rect & rc, Texture_Asset * texture, bool is_hovered, bool is_hot );
	void draw_image( const Rect & rc, Texture_Asset * texture );

	// #ui - remove all these references to rc_ui and rc_client - use the member vars we already have (??)
	virtual void draw( const Rect & rc_ui, const Rect & rc_client, bool is_hovered, bool is_hot );

	void draw_panel( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot );
	void draw_caption( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot );
	void draw_button( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot );
	void draw_check( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot );
	void draw_divider( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot );
	void draw_spacer( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot );
	void draw_image( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot );
	void draw_label( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot );
	void draw_slider( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot );
	void draw_text( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot );
	void draw_radio( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot );
	void draw_progress( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot );
	void draw_list( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot );
	void draw_dropdown( const Rect& rc_ui, const Rect& rc_client, bool is_hovered, bool is_hot );

	static float_t get_default_width( e_ui_control_type_t type );
	static float_t get_default_height( e_ui_control_type_t type );
	Vec2 get_control_inner_margins();

	float_t default_width = 0.f;
	float_t default_height = 0.f;
};

}
