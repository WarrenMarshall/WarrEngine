
namespace war
{

// ----------------------------------------------------------------------------
// a primitive or a polygonal shape

struct Tiled_Object
{
	e_sc_prim_type_t collision_type = e_sc_prim_type::aabb;

	Rect rc;
	float_t radius;
	std::vector<Vec2> vertices;
	std::string type;

	void rotate( float_t angle )
	{
		Matrix mtx;
		mtx.rotate( angle );

		for( auto& v : vertices )
		{
			v = mtx.transform_vec2( v );
		}
	}
};

// ----------------------------------------------------------------------------
// object group

struct Tiled_Object_Group
{
	std::string tag;
	std::vector<Tiled_Object> objects;
	bool is_visible = true;
};


// ----------------------------------------------------------------------------

struct Tile_Set_Asset : Asset
{
	// ----------------------------------------------------------------------------
	// a tile definition

	struct Tile_Def
	{
		Texture_Asset texture;
		std::vector<Tiled_Object> objects;
	};

	int32_t tile_width = 0, tile_height = 0, tile_count = 0, columns = 0;
	Texture_Asset* texture = nullptr;	// the source texture containing the tile set atlas
	std::vector<Tile_Def> tile_definitions;

	virtual bool create() override;
};

}
