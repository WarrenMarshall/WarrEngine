
namespace war
{

struct Render_State final
{
	Render_State();

	e_align_t align = e_align::left;
	float angle = 0.f;
	Color color = Color::white;
	float glow = 0.f;
	int pick_id = 0;
	Vec2 scale = { 1.f, 1.f };
	Vec2 uv_tiling = { 1.f, 1.f };
	float z = zdepth_background;
	float z_bias = 0;
	float anim_offset = 0.f;
	Primitive_Batch_Group* batch_render_target = nullptr;
	Font_Asset* font = nullptr;		// set in ctor

	void set_from_opt( Render_State_Optional& rso );
};

struct Render_State_Optional final
{
	std::optional<e_align_t> align = std::nullopt;
	std::optional<float> angle = std::nullopt;
	std::optional<Color> color = std::nullopt;
	std::optional<float> glow = std::nullopt;
	std::optional<int> pick_id = std::nullopt;
	std::optional<Vec2> scale = std::nullopt;
	std::optional<Vec2> uv_tiling = std::nullopt;
	std::optional<float> z = std::nullopt;
	std::optional<float> z_bias = std::nullopt;
	std::optional<Font_Asset*> font = std::nullopt;
};

}
