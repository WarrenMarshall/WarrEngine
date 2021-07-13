
namespace war
{

struct render_state
{
	render_state();

	e_align align = align::left;
	float angle = 0.f;
	color color = color::white;
	float glow = 0.f;
	int pick_id = 0;
	vec2 scale = { 1.f, 1.f };
	vec2 uv_tiling = { 1.f, 1.f };
	float z = zdepth_background;
	float z_bias = 0;
	float anim_offset = 0.f;
	render_batch_group* batch_render_target = nullptr;
	font_asset* font = nullptr;		// set in ctor

	void set_from_opt( render_state_optional& rso );
};

struct render_state_optional
{
	std::optional<e_align> align = std::nullopt;
	std::optional<float> angle = std::nullopt;
	std::optional<color> color = std::nullopt;
	std::optional<float> glow = std::nullopt;
	std::optional<int> pick_id = std::nullopt;
	std::optional<vec2> scale = std::nullopt;
	std::optional<vec2> uv_tiling = std::nullopt;
	std::optional<float> z = std::nullopt;
	std::optional<float> z_bias = std::nullopt;
	std::optional<font_asset*> font = std::nullopt;
};

}
