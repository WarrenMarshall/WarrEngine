
namespace war
{

struct ui_control
{
	e_ui_control_type type = ui_control_type::none;

	hash tag;
	std::string text;
	e_align text_align = align::hcenter | align::vcenter;
	color text_color = color::white;
	color primary_color = color::black;
	slice_def_asset* slice_def = nullptr;
	int idx = -1;
	size_t total = 0;
	texture_asset* image = nullptr;
	float image_scale = 1.f;
	f_draw_control func_draw_control = nullptr;

	// expressed as a 0-1 percentage.  So 0.2 would be 5 intervals, 0.1 would be 10, etc.
	float interval = 0.f;

	// the ui and client rectangles
	rect rc_ui = {};
	rect rc_client = {};

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

	ui_control( hash tag = hash_none );

	// #ui - remove all these references to rc_ui and rc_client - use the member vars we already have
	void draw_slice_def( const rect& rc_ui, bool is_hovered, bool is_hot );
	void draw_text( const rect& rc_client, const color& color, bool is_hovered, bool is_hot, const std::string& text );
	void draw_texture( const rect& rc, texture_asset* texture, bool is_hovered, bool is_hot );
	void draw_image( const rect& rc, texture_asset* texture );

	virtual void draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot );

	virtual vec2 get_control_inner_margins();

	float default_width = 0.f;
	float default_height = 0.f;
};

// ----------------------------------------------------------------------------

struct ui_panel_control final : ui_control
{
	ui_panel_control( hash tag = hash_none );

	virtual void draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float get_default_width();
	static float get_default_height();
};

struct ui_caption_control final : ui_control
{
	ui_caption_control( hash tag = hash_none );

	virtual void draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float get_default_width();
	static float get_default_height();
};

struct ui_button_control final : ui_control
{
	ui_button_control( hash tag = hash_none );

	virtual void draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float get_default_width();
	static float get_default_height();
};

struct ui_check_control final : ui_control
{
	ui_check_control( hash tag = hash_none );


	virtual void draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float get_default_width();
	static float get_default_height();
};

struct ui_divider_control final : ui_control
{
	ui_divider_control( hash tag = hash_none );

	virtual void draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float get_default_width();
	static float get_default_height();
	virtual vec2 get_control_inner_margins() override;
};

struct ui_spacer_control final : ui_control
{
	ui_spacer_control( hash tag = hash_none );

	virtual void draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float get_default_width();
	static float get_default_height();
	virtual vec2 get_control_inner_margins() override;
};


struct ui_image_control final : ui_control
{
	ui_image_control( hash tag = hash_none );

	virtual void draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float get_default_width();
	static float get_default_height();
};

struct ui_label_control final : ui_control
{
	ui_label_control( hash tag = hash_none );

	virtual void draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float get_default_width();
	static float get_default_height();
};

struct ui_slider_control final : ui_control
{
	ui_slider_control( hash tag = hash_none );

	virtual void draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot ) override;
	void draw_slider_thumb( const rect& rc_client );
	void draw_slider_tick_marks( const rect& rc_ui, const rect& rc_client );

	static float get_default_width();
	static float get_default_height();
	virtual vec2 get_control_inner_margins() override;
};

struct ui_text_control final : ui_control
{
	ui_text_control( hash tag = hash_none );

	virtual void draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float get_default_width();
	static float get_default_height();
};

struct ui_radio_control final : ui_control
{
	ui_radio_control( hash tag = hash_none );

	virtual void draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float get_default_width();
	static float get_default_height();
};

struct ui_progress_control final : ui_control
{
	ui_progress_control( hash tag = hash_none );


	virtual void draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float get_default_width();
	static float get_default_height();
};

struct ui_list_control final : ui_control
{
	ui_list_control( hash tag = hash_none );

	virtual void draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float get_default_width();
	static float get_default_height();
};

struct ui_dropdown_control final : ui_control
{
	ui_dropdown_control( hash tag = hash_none );

	virtual void draw( const rect& rc_ui, const rect& rc_client, bool is_hovered, bool is_hot ) override;

	static float get_default_width();
	static float get_default_height();
};

}
