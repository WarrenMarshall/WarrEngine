#pragma once

struct a_mesh_vertex
{
	w_vec3 pos;
	w_uv uv;
};

// ----------------------------------------------------------------------------

struct a_mesh : i_asset
{
	declare_find_func( a_mesh )

	a_texture* tex = nullptr;

	std::vector<a_mesh_vertex> mesh_verts;
	//std::vector<w_render_vertex> render_verts;		// triangles, so in groups of 3

	virtual bool create_internals() override;
};
