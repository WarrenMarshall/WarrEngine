
namespace war
{

// this class exists because cursors need to know where their hot spot is.

struct Cursor_Asset : Asset
{
	Vec2 hotspot_offset = Vec2( 0, 0 );
	Texture_Asset* texture = nullptr;
};

}
