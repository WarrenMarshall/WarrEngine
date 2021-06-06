
namespace war
{

// ----------------------------------------------------------------------------

constexpr unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
constexpr unsigned FLIPPED_VERTICALLY_FLAG = 0x40000000;
constexpr unsigned FLIPPED_DIAGONALLY_FLAG = 0x20000000;

// ----------------------------------------------------------------------------

struct tile_map_asset final : asset
{
	struct tile
	{
		tile( int idx, e_tile_flags flags );

		int idx;
		e_tile_flags flags;
	};

	struct layer
	{
		std::string tag;
		std::vector<tile> tiles;
		bool is_visible = true;
	};

	int width = 0;
	int height = 0;
	int tile_width = 0;
	int tile_height = 0;
	std::vector<layer> layers;

	virtual bool create() override;
};

}
