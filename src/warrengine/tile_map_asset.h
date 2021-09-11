
namespace war
{

// ----------------------------------------------------------------------------

constexpr uint32_t FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
constexpr uint32_t FLIPPED_VERTICALLY_FLAG = 0x40000000;
constexpr uint32_t FLIPPED_DIAGONALLY_FLAG = 0x20000000;

// ----------------------------------------------------------------------------

struct Tile_Map_Asset final : Asset
{
	// ----------------------------------------------------------------------------
	// a tile within a chunk

	struct Tile final
	{
		static constexpr int32_t empty = -1;

		Tile( int32_t idx, int32_t x_idx, int32_t y_idx, e_tile_flags_t flags );

		int32_t idx = 0;
		int32_t x_idx = 0, y_idx = 0;		// coordinates from the top left of the tile map
		e_tile_flags_t flags = e_tile_flags::none;
	};

	// ----------------------------------------------------------------------------
	// chunks
	//
	// chunks are stored as a collection of tiles

	struct Chunk final
	{
		static constexpr int32_t default_chunk_sz = 16;

		// the rectangular area on the tilemap where this chunk sits
		Rect tilemap_bounds = { 0, 0, default_chunk_sz, default_chunk_sz };

		std::vector<Tile> tiles = {};
	};

	// ----------------------------------------------------------------------------
	// layer
	//
	// layers are stored as collections of chunks.

	struct Layer final
	{
		std::string tag;
		std::vector<Chunk> chunks = {};
		bool is_visible = true;
	};

	// global info about the tile map

	int32_t width = 0;
	int32_t height = 0;
	int32_t tile_sz = 0;
	bool is_infinite = false;
	std::vector<Layer> layers;
	std::vector<Tiled_Object_Group> object_groups;

	virtual bool create() override;
};

}
