#pragma once

struct a_mesh : i_asset
{
	declare_find_func( a_mesh )

	a_raw_image_data* tex = nullptr;
	std::vector<w_render_vertex> render_verts;		// triangles, so in groups of 3

	virtual ~a_mesh() override;

	virtual void clean_up_internals() override;
	virtual bool create_internals() override;
};
