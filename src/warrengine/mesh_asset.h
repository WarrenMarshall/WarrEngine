
namespace war
{

// ----------------------------------------------------------------------------

struct mesh_asset final : asset
{
	std::unordered_map<texture_asset*, std::vector<render_triangle>> texture_to_triangles;
	virtual bool create() override;
};

}
