
namespace war
{

// ----------------------------------------------------------------------------

constexpr ui32 FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
constexpr ui32 FLIPPED_VERTICALLY_FLAG = 0x40000000;
constexpr ui32 FLIPPED_DIAGONALLY_FLAG = 0x20000000;

// ----------------------------------------------------------------------------

struct Tile_Map_Asset final : Asset
{
	// ----------------------------------------------------------------------------
	// a tile within a chunk

	struct Tile final
	{
		static constexpr i32 empty = -1;

		Tile( i32 idx, i32 x_idx, i32 y_idx, e_tile_flags flags );

		i32 idx = 0;
		i32 x_idx = 0, y_idx = 0;		// coordinates from the top left of the tile map
		e_tile_flags flags = e_tile_flags::none;
	};

	// ----------------------------------------------------------------------------
	// chunks
	//
	// chunks are stored as a collection of tiles

	struct Chunk final
	{
		static constexpr i32 default_chunk_sz = 16;

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
		struct
		{
			bool is_visible : 1 = true;
			bool should_draw : 1 = true;
		} flags;
		hash tag;
		std::vector<Chunk> chunks = {};
	};

	// global info about the tile map

	i32 width = 0;
	i32 height = 0;
	i32 tile_sz = 0;
	bool is_infinite = false;
	std::vector<Layer> layers;
	std::vector<Tiled_Object_Group> object_groups;

	virtual bool create() override;
};

}
