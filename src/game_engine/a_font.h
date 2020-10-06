#pragma once

struct w_font_char
{
	float w = 0.0f;
	float h = 0.0f;
	float xoffset = 0.0f;
	float yoffset = 0.0f;
	float xadvance = 0.0f;
	std::unique_ptr<a_subtexture> subtex = nullptr;
};

// ----------------------------------------------------------------------------

constexpr int max_font_chars = 128;

struct a_font_def : i_asset
{
	// the font texture this font definition is pulling from
	a_texture* texture = nullptr;

	// this height value is the largest one found in the font. using this is
	// guaranteed to enclose any line of text.
	int max_height = 0;

	// using an array here to maximize look ups later on. char values
	// become indices into this array.
	w_font_char char_map[ max_font_chars ];

	virtual bool create_internals() override;
};

// ----------------------------------------------------------------------------

struct a_font : i_asset
{
	a_font_def* font_def = nullptr;

	w_vec2 get_string_extents( const std::string_view text );
};