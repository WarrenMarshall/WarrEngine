#pragma once

struct w_render_vertex
{
	w_render_vertex( const w_vec2& pos, const w_uv& uv, const w_color& color, const float emissive );
	w_render_vertex( const w_vec3& pos, const w_uv& uv, const w_color& color, const float emissive );

	float x, y, z;
	float u, v;
	float r, g, b, a;
	float e;
	float t;
};

