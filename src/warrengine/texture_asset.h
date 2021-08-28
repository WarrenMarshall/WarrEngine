
namespace war
{

// a texture is a subsection of a texture_source_asset. it can
// represent the entire source texture or a sub rectangle of it.
//
// these are what gets passed to the renderer functions.

struct Texture_Asset final : Asset
{
	// the raw_image asset this texture sources from
	Texture_Source_Base* get_src_texture() const;

	// the rectangle on the raw image that defines this texture
	Rect rc = {};

	// the uv coordinates that define this image within the raw_image
	Vec2 uv00 = { 0.f, 0.f };
	Vec2 uv11 = { 1.f, 1.f };

	// animation controls
	std::vector<Texture_Asset*> frames;
	Tween frame_tween;
	e_tween_type tween_type = tween_type::loop;
	int frames_per_second = 0;

	Texture_Asset() = default;
	Texture_Asset( std::string_view src_texture_tag );
	Texture_Asset( std::string_view src_texture_tag, const Rect& rc_tex );
	Texture_Asset( std::vector<Texture_Asset*>& frames, e_tween_type tween_type, int frames_per_second );

	Texture_Source_Base* find_texture_source_base( std::string_view src_texture_tag );

	[[nodiscard]] Texture_Asset* get_frame( float anim_offset );
	void randomize();
	bool is_animated() const;

	float width();
	float height();

private:
	Texture_Source_Base* src_texture = nullptr;
};

}
