
// there are one of these assets in the cache for each image
// loaded from the disk. this is what gets uploaded to opengl
// and assigned an id.
//
// these are NEVER sent to w_render, they exist solely
// to represent the raw texture data.
//
// games should never interact direcly with this class.
// this is engine side only.

namespace war
{

struct texture_source_asset final : texture_source_base
{
	e_tiling tiling = tiling::repeat;

	virtual bool create() override;
};

}
