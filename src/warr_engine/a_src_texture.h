#pragma once

// there are one of these assets in the cache for each image
// loaded from the disk. this is what gets uploaded to opengl
// and assigned an id.
//
// these are NEVER sent to the renderer, they exist solely
// to represent the raw texture data.
//
// games should never interact direcly with this class.
// this is engine side only.

struct a_src_texture : i_asset
{
	declare_find_func( a_src_texture )

	unsigned int gl_id = 0;
	float w = 0.0f;
	float h = 0.0f;
	e_tiling tiling = tiling::clamp;

	unsigned char* color_data = nullptr;

	virtual ~a_src_texture() override;

	virtual void clean_up_internals() override;
	virtual bool create_internals() override;
	virtual void finalize_after_loading() override;
};
