
namespace war
{

// ----------------------------------------------------------------------------

constexpr unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
constexpr unsigned FLIPPED_VERTICALLY_FLAG = 0x40000000;
constexpr unsigned FLIPPED_DIAGONALLY_FLAG = 0x20000000;

// ----------------------------------------------------------------------------

struct tile_map_asset final : asset
{
	// a tile within a layer

	struct tile
	{
		tile( size_t idx, e_tile_flags flags );

		size_t idx;
		e_tile_flags flags;
	};

	// a layer of tiles

	struct layer
	{
		std::string tag;
		std::vector<tile> tiles;
		bool is_visible = true;
	};

	struct object
	{
		rect rc;
	};

	// object group

	struct object_group
	{
		std::string tag;
		std::vector<object> objects;
		bool is_visible = true;
	};

	// global info about the tile map

	size_t width = 0;
	size_t height = 0;
	size_t tile_sz = 0;
	std::vector<layer> layers;
	std::vector<object_group> object_groups;

	virtual bool create() override;
};

}
