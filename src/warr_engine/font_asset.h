
namespace war
{

// ----------------------------------------------------------------------------

constexpr int max_font_chars = 256;

struct font_def_asset final : asset
{
	struct glyph
	{
		int16 w = 0;
		int16 h = 0;
		int16 xoffset = 0;
		int16 yoffset = 0;
		int16 xadvance = 0;

		// this is not in the main asset cache as it would be inefficient to flood
		// the asset cache with an a_texture for every glyph in a font.

		texture_asset glyph_texture;
	};

	// the font texture this font definition is pulling from
	texture_source_asset* src_texture = nullptr;

	// this height value is the largest one found in the font. using this is
	// guaranteed to enclose any line of text.
	int16 max_height = 0;

	// using an array here to maximize look ups later on. char values
	// become indices into this array.
	glyph char_map[ max_font_chars ];

	virtual bool create() override;
};

// ----------------------------------------------------------------------------

struct font_asset : asset
{
	font_def_asset* font_def = nullptr;

	[[nodiscard]] vec2 get_string_extents( std::string_view text ) const;
	[[nodiscard]] float get_max_height();
};

}
