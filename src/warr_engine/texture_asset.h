
namespace war
{

// a texture is a subsection of a texture_source_asset. it can
// represent the entire source texture or a sub rectangle of it.
//
// these are what gets passed to the renderer functions.

struct texture_asset final : asset
{
	// the raw_image asset this texture sources from
	texture_source_base* get_src_texture() const;

	// the rectangle on the raw image that defines this texture
	rect rc = {};

	// the uv coordinates that define this image within the raw_image
	vec2 uv00 = { 0.f, 0.f };
	vec2 uv11 = { 1.f, 1.f };

	// animation controls
	std::vector<texture_asset*> frames;
	tween frame_tween;
	e_tween_type tween_type = tween_type::loop;
	int frames_per_second = 0;

	texture_asset() = default;
	texture_asset( std::string_view src_texture_tag );
	texture_asset( std::string_view src_texture_tag, const rect& rc_tex );
	texture_asset( std::vector<texture_asset*>& frames, e_tween_type tween_type, int frames_per_second );

	texture_source_base* find_texture_source_base( std::string_view src_texture_tag );

	[[nodiscard]] texture_asset* get_frame( float anim_offset );
	void randomize();
	bool is_animated() const;

	float width();
	float height();

private:
	texture_source_base* src_texture = nullptr;
};

}
