
namespace war
{

struct Render_State final
{
	Render_State();

	e_align align = e_align::left;
	f32 pct = 100.f;
	f32 angle = 0.f;
	Color color = Color::white;
	f32 glow = 0.f;
	i32 pick_id = 0;
	Vec2 scale = { 1.f, 1.f };
	Vec2 uv_tiling = { 1.f, 1.f };
	f32 z = zdepth_background;
	f32 z_bias = 0;
	f32 anim_offset = 0.f;
	Primitive_Batch_Group* batch_render_target = nullptr;
	Font_Asset* font = nullptr;		// set in ctor

	void set_from_opt( Render_State_Optional& rso );
};

struct Render_State_Optional final
{
	std::optional<e_align> align = std::nullopt;
	std::optional<f32> pct = std::nullopt;
	std::optional<f32> angle = std::nullopt;
	std::optional<Color> color = std::nullopt;
	std::optional<f32> glow = std::nullopt;
	std::optional<int> pick_id = std::nullopt;
	std::optional<Vec2> scale = std::nullopt;
	std::optional<Vec2> uv_tiling = std::nullopt;
	std::optional<f32> z = std::nullopt;
	std::optional<f32> z_bias = std::nullopt;
	std::optional<Font_Asset*> font = std::nullopt;
};

}
