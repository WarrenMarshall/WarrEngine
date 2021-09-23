
namespace war
{

struct UI_Result final
{
	e_im_result code = e_im_result::none;

	// the mouse position, normalized to within the client
	// rect of the control.
	Vec2 click_pos = {};
	Vec2 click_pct = {};

	void operator=( e_im_result res );
};

}
