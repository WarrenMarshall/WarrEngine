
namespace war
{

struct Mesh_Asset : Asset
{
	// groups up triangles into buckets, keyed off of texture_asset pointers
	std::unordered_map<Texture_Asset*, std::vector<Render_Triangle>> texture_to_triangles;

	virtual bool create() override;
};

}
