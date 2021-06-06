
namespace war
{

// this class exists because cursors need to know where their hot spot is.

struct cursor_asset final : asset
{
	vec2 hotspot_offset = vec2( 0, 0 );
	texture_asset* texture = nullptr;
};

}
