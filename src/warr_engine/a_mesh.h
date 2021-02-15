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

	virtual bool create_internals() override;
};
