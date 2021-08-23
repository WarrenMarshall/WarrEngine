
namespace war
{

// ----------------------------------------------------------------------------

constexpr unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
constexpr unsigned FLIPPED_VERTICALLY_FLAG = 0x40000000;
constexpr unsigned FLIPPED_DIAGONALLY_FLAG = 0x20000000;

// ----------------------------------------------------------------------------

struct tile_map_asset final : asset
{
	// ----------------------------------------------------------------------------
	// a tile within a chunk

	struct tile
	{
		static constexpr int empty = -1;

		tile( int idx, int x_idx, int y_idx, e_tile_flags flags );

		int idx = 0;
		int x_idx = 0, y_idx = 0;		// coordinates from the top left of the tile map
		e_tile_flags flags = tile_flags::none;
	};

	// ----------------------------------------------------------------------------
	// chunks
	//
	// chunks are stored as a collection of tiles

	struct chunk
	{
		static constexpr int default_chunk_sz = 16;

		// the rectangular area on the tilemap where this chunk sits
		rect tilemap_bounds = { 0, 0, default_chunk_sz, default_chunk_sz };

		std::vector<tile> tiles = {};
	};

	// ----------------------------------------------------------------------------
	// layer
	//
	// layers are stored as collections of chunks.

	struct layer
	{
		std::string tag;
		std::vector<chunk> chunks = {};
		bool is_visible = true;
	};

	// global info about the tile map

	int width = 0;
	int height = 0;
	int tile_sz = 0;
	bool is_infinite = false;
	std::vector<layer> layers;
	std::vector<tiled_object_group> object_groups;

	virtual bool create() override;
};

}
