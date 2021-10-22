
namespace war
{

// ----------------------------------------------------------------------------

constexpr uint8 max_font_chars = 255;

struct Font_Def_Asset final : Asset
{
	struct Glyph final
	{
		uint8 w = 0;
		uint8 h = 0;
		uint8 xoffset = 0;
		uint8 yoffset = 0;
		uint8 xadvance = 0;

		// this is not in the main asset cache as it would be inefficient to flood
		// the asset cache with an a_texture for every glyph in a font.

		Texture_Asset glyph_texture;
	};

	// the font texture this font definition is pulling from
	Texture_Source_Asset* src_texture = nullptr;

	// this height value is the largest one found in the font. using this is
	// guaranteed to enclose any line of text.
	uint8 max_height = 0;

	// using an array here to maximize look ups later on. char values
	// become indices into this array.
	Glyph char_map[ max_font_chars ];

	virtual bool create() override;
};

// ----------------------------------------------------------------------------

struct Font_Asset final : Asset
{
	Font_Def_Asset* font_def = nullptr;

	[[nodiscard]] Vec2 get_string_extents( std::string_view text ) const;
	[[nodiscard]] f32 get_max_height();
};

}
