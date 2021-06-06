
namespace war
{

// ----------------------------------------------------------------------------

struct mesh_asset final : asset
{
	struct vertex
	{
		texture_asset* texture = nullptr;
		vec3 pos;
		vec2 uv;
	};

	std::vector<vertex> mesh_verts;

	virtual bool create() override;
};

}
