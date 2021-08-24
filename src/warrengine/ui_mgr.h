
namespace war
{

struct ui_mgr final
{
	static unsigned automatic_id;

	cursor_asset* mouse_cursor = nullptr;

	// if a scene doesn't have a ui_callback, this is used. simplifies a lot of
	// code that no longer needs to check for null pointers.
	ui_callback default_callback;

	// convenience pointers that are set by the scene_mgr before the UI is drawn for each scene
	ui_callback* current_callback = nullptr;

	// which UI control the mouse is hovering over
	hash hover_tag = hash_none;

	// a hot id is one the user has clicked the mouse on but hasn't unclicked yet
	hash hot_tag = hash_none;

	std::unique_ptr<ui_control> current_control = nullptr;

	// the results from the last control processed
	ui_result result = {};

	// a global caret blinker, for all text controls to share
	tween caret_blink_tween;

	// if a control is meant to retain focus across frames, store info about it here.
	struct
	{
		hash tag = hash_none;
		e_ui_control_type type = ui_control_type::none;
	} focused;

	// layout stack control
	std::vector<rect> layout_stack;

	ui_mgr();

	void reset();

	ui_mgr* panel_control( hash tag = hash_none );
	ui_mgr* caption_control( hash tag = hash_none );
	ui_mgr* button_control( hash tag = hash_none );
	ui_mgr* check_control( hash tag = hash_none );
	ui_mgr* divider_control( hash tag = hash_none );
	ui_mgr* spacer_control( hash tag = hash_none );
	ui_mgr* image_control( hash tag = hash_none );
	ui_mgr* label_control( hash tag = hash_none );
	ui_mgr* slider_control( hash tag = hash_none );
	ui_mgr* text_control( hash tag = hash_none );
	ui_mgr* radio_control( hash tag = hash_none );
	ui_mgr* progress_control( hash tag = hash_none );
	ui_mgr* list_control( hash tag = hash_none );
	ui_mgr* dropdown_control( hash tag = hash_none );

private:
	template<typename T>
	ui_mgr* do_control( hash tag = hash_none )
	{
		current_control = std::make_unique<T>();
		current_control->tag = tag;
		current_control->default_width = T::get_default_width();
		current_control->default_height = T::get_default_height();

		if( current_control->type == ui_control_type::panel )
		{
			current_control->default_width = layout_top().w;
			current_control->default_height = layout_top().h;
		}

		if( tag == hash_none )
		{
			// if the control is active and does NOT have a specific tag, auto
			// generate an id for it
			if( current_control->is_active )
			{
				current_control->tag = automatic_id++;
			}
		}

		set_size( { vec2::defaulted, vec2::defaulted } );

		return this;
	}
public:

	ui_mgr* set_text( std::string text );
	ui_mgr* center_control_on_screen();
	ui_mgr* adjust_layout_to_client_area();
	ui_mgr* set_text_align( e_align align );
	ui_mgr* set_text_color( color color );
	ui_mgr* set_color( color color );
	ui_mgr* set_image( std::string_view tex_tag );
	ui_mgr* set_image( texture_asset* tex );
	ui_mgr* set_func_draw( f_draw_control func_draw_control );
	ui_mgr* set_slice_def( std::string_view slice_def_name );
	ui_mgr* set_rect( const rect& rect );
	ui_mgr* cut_top( float sz = 0.f );
	ui_mgr* cut_bottom( float sz = 0.f );
	ui_mgr* cut_left( float sz = 0.f );
	ui_mgr* cut_right( float sz = 0.f );
	ui_mgr* set_pos( const vec2& pos );
	ui_mgr* set_size( const vec2& sz );
	ui_mgr* set_interval( float interval );
	ui_mgr* set_idx( int idx );

	rect compute_client_rect_from_ui_rect( rect rc_ui );
	bool done();
	void end_static_control();
	void end_active_control();

	void draw_topmost();
	bool is_mouse_inside( rect& rc );

	[[nodiscard]] vec2 get_click_offset( bool is_hovered, bool is_hot );
	[[nodiscard]] color get_adjusted_color( const color& base_color, bool is_hovered, bool is_hot );

	ui_mgr* layout_init();
	ui_mgr* layout_init( rect rc );
	ui_mgr* layout_push();
	ui_mgr* layout_push( rect rc );
	ui_mgr* layout_pop();
	rect layout_top();
	rect* layout_top_ptr();

private:
	void update_im_state( ui_control* control, bool is_hovered, bool is_hot );
	void draw( ui_control* control, bool is_hovered, bool is_hot );
};

}
