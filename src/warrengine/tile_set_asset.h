
namespace war
{

// ----------------------------------------------------------------------------
// a primitive or a polygonal shape

struct tiled_object
{
	e_sc_prim_type collision_type = sc_prim_type::aabb;

	rect rc;
	float radius;
	std::vector<vec2> vertices;
	std::string type;

	void rotate( float angle )
	{
		matrix mtx;
		mtx.rotate( angle );

		for( auto& v : vertices )
		{
			v = mtx.transform_vec2( v );
		}
	}
};

// ----------------------------------------------------------------------------
// object group

struct tiled_object_group
{
	std::string tag;
	std::vector<tiled_object> objects;
	bool is_visible = true;
};


// ----------------------------------------------------------------------------

struct tile_set_asset final : asset
{
	// ----------------------------------------------------------------------------
	// a tile definition

	struct tile_def
	{
		texture_asset texture;
		std::vector<tiled_object> objects;
	};

	int tile_width = 0, tile_height = 0, tile_count = 0, columns = 0;
	texture_asset* texture = nullptr;	// the source texture containing the tile set atlas
	std::vector<tile_def> tile_definitions;

	virtual bool create() override;
};

}
