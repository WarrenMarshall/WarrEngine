
namespace war
{

struct ui_result final
{
	e_im_result code = im_result::none;

	// the mouse position, normalized to within the client
	// rect of the control.
	vec2 click_pos = {};
	vec2 click_pct = {};

	void operator=( const e_im_result res );
};

}
