
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
		static const int empty = -1;

		tile( int idx, int x_idx, int y_idx, e_tile_flags flags );

		int idx;
		int x_idx, y_idx;		// coordinates from the top left of the tile map
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
		e_sc_prim_type collision_type = sc_prim_type::aabb;
		rect rc = {};
		float radius = 0.f;
		std::vector<vec2> vertices;
		std::string type;

		void rotate_vertices( float angle )
		{
			matrix mtx;
			mtx.rotate( angle );

			for( auto& v : vertices )
			{
				v = mtx.transform_vec2( v );
			}
		}
	};

	// object group

	struct object_group
	{
		std::string tag;
		std::vector<object> objects;
		bool is_visible = true;
	};

	// global info about the tile map

	int width = 0;
	int height = 0;
	int tile_sz = 0;
	std::vector<layer> layers;
	std::vector<object_group> object_groups;

	virtual bool create() override;
};

}
