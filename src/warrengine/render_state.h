
namespace war
{

struct Render_State
{
	Render_State();

	e_align_t align = e_align::left;
	float_t pct = 100.f;
	float_t angle = 0.f;
	Color color = Color::white;
	float_t glow = 0.f;
	int32_t pick_id = 0;
	Vec2 scale = { 1.f, 1.f };
	Vec2 uv_tiling = { 1.f, 1.f };
	float_t z = zdepth_background;
	float_t z_bias = 0;
	float_t anim_offset = 0.f;
	Primitive_Batch_Group* batch_render_target = nullptr;
	Font_Asset* font = nullptr;		// set in ctor

	void set_from_opt( Render_State_Optional& rso );
};

struct Render_State_Optional
{
	std::optional<e_align_t> align = std::nullopt;
	std::optional<float_t> pct = std::nullopt;
	std::optional<float_t> angle = std::nullopt;
	std::optional<Color> color = std::nullopt;
	std::optional<float_t> glow = std::nullopt;
	std::optional<int> pick_id = std::nullopt;
	std::optional<Vec2> scale = std::nullopt;
	std::optional<Vec2> uv_tiling = std::nullopt;
	std::optional<float_t> z = std::nullopt;
	std::optional<float_t> z_bias = std::nullopt;
	std::optional<Font_Asset*> font = std::nullopt;
};

}
