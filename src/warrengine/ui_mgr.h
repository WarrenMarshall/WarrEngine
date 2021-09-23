
namespace war
{

struct UI_Mgr final
{
	static uint32_t automatic_id;

	Cursor_Asset* mouse_cursor = nullptr;

	// if a scene doesn't have a ui_callback, this is used. simplifies a lot of
	// code that no longer needs to check for null pointers.
	UI_Callback default_callback;

	// convenience pointers that are set by the scene_mgr before the UI is drawn for each scene
	UI_Callback* current_callback = nullptr;

	// which UI control the mouse is hovering over
	hash hover_tag = hash_none;

	// a hot id is one the user has clicked the mouse on but hasn't unclicked yet
	hash hot_tag = hash_none;

	std::unique_ptr<UI_Control> current_control = nullptr;

	// the results from the last control processed
	UI_Result result = {};

	// a global caret blinker, for all text controls to share
	Tween caret_blink_tween;

	// if a control is meant to retain focus across frames, store info about it here.
	struct
	{
		hash tag = hash_none;
		e_ui_control_type type = e_ui_control_type::none;
	} focused;

	// layout stack control
	std::vector<Rect> layout_stack;

	UI_Mgr();

	void reset();

	UI_Mgr* panel_control( hash tag = hash_none );
	UI_Mgr* caption_control( hash tag = hash_none );
	UI_Mgr* button_control( hash tag = hash_none );
	UI_Mgr* check_control( hash tag = hash_none );
	UI_Mgr* divider_control( hash tag = hash_none );
	UI_Mgr* spacer_control( hash tag = hash_none );
	UI_Mgr* image_control( hash tag = hash_none );
	UI_Mgr* label_control( hash tag = hash_none );
	UI_Mgr* slider_control( hash tag = hash_none );
	UI_Mgr* text_control( hash tag = hash_none );
	UI_Mgr* radio_control( hash tag = hash_none );
	UI_Mgr* progress_control( hash tag = hash_none );
	UI_Mgr* list_control( hash tag = hash_none );
	UI_Mgr* dropdown_control( hash tag = hash_none );

private:
	UI_Mgr* do_control( e_ui_control_type control_type, hash tag = hash_none );

public:

	UI_Mgr* set_text( std::string text );
	UI_Mgr* center_control_on_screen();
	UI_Mgr* adjust_layout_to_client_area();
	UI_Mgr* set_text_align( e_align align );
	UI_Mgr* set_text_color( Color color );
	UI_Mgr* set_color( Color color );
	UI_Mgr* set_image( std::string_view tex_tag );
	UI_Mgr* set_image( const Texture_Asset* tex );
	UI_Mgr* set_func_draw( f_draw_control func_draw_control );
	UI_Mgr* set_slice_def( std::string_view slice_def_name );
	UI_Mgr* set_rect( const Rect& rect );
	UI_Mgr* cut_top( float_t sz = 0.f );
	UI_Mgr* cut_bottom( float_t sz = 0.f );
	UI_Mgr* cut_left( float_t sz = 0.f );
	UI_Mgr* cut_right( float_t sz = 0.f );
	UI_Mgr* set_pos( const Vec2& pos );
	UI_Mgr* set_size( const Vec2& sz );
	UI_Mgr* set_interval( float_t interval );
	UI_Mgr* set_idx( int32_t idx );

	Rect compute_client_rect_from_ui_rect( Rect rc_ui );
	bool done();
	void end_static_control();
	void end_active_control();

	void draw_topmost();
	bool is_mouse_inside( Rect& rc );

	[[nodiscard]] Vec2 get_click_offset( bool is_hovered, bool is_hot );
	[[nodiscard]] Color get_adjusted_color( const Color& base_color, bool is_hovered, bool is_hot );

	UI_Mgr* layout_init();
	UI_Mgr* layout_init( Rect rc );
	UI_Mgr* layout_push();
	UI_Mgr* layout_push( Rect rc );
	UI_Mgr* layout_pop();
	Rect layout_top();
	Rect* layout_top_ptr();

private:
	void update_im_state( UI_Control* control, bool is_hovered, bool is_hot );
	void draw( UI_Control* control, bool is_hovered, bool is_hot );
};

}
