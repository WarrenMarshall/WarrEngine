#pragma once

struct a_mesh_vertex
{
	a_texture* texture = nullptr;
	w_vec3 pos;
	w_uv uv;
};

// ----------------------------------------------------------------------------

struct a_mesh : i_asset
{
	declare_find_func( a_mesh )

	std::vector<a_mesh_vertex> mesh_verts;

	virtual bool create_internals() override;
};
