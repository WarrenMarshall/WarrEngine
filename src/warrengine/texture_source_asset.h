
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

struct Texture_Source_Asset final : Texture_Source_Base
{
	// how the edges of the texture are handled
	e_tiling tiling = e_tiling::repeat;

	// set to false if you never want this texture to generate mipmaps
	bool use_mipmaps = true;

	virtual bool create() override;
};

}
