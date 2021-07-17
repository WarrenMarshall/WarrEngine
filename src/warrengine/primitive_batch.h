
namespace war
{

// ----------------------------------------------------------------------------

struct primitive_batch
{
	// the max per batch before we force a flush and set up a new batch.
	static const int max_elements_per_draw_call = 15'000;

	primitive_batch() = default;
	primitive_batch( primitive_batch& ) = delete;
	primitive_batch( e_render_prim render_prim );

	void init( e_render_prim render_prim );
	vertex_array_object vao[ draw_call::max ];

	void add_quad( texture_asset* texture, const render_vertex* v0, const render_vertex* v1, const render_vertex* v2, const render_vertex* v3 );
	void add_triangle( texture_asset* texture, const render_vertex* v0, const render_vertex* v1, const render_vertex* v2 );
	void add_line( texture_asset* texture, const render_vertex* v0, const render_vertex* v1 );
	void add_point( texture_asset* texture, const render_vertex* v0 );

	bool is_empty();

private:
	virtual void add_vert( e_draw_call draw_call, texture_asset* texture, const render_vertex* render_vert );
};

// ----------------------------------------------------------------------------

struct render_batch_group
{
	std::array<primitive_batch, 4> batches;

	bool invalidated = false;

	void init();
	bool is_empty();
	void flush_and_reset( e_draw_call draw_call );
	void flush_and_reset_internal( e_draw_call draw_call );

	void add_quad( texture_asset* texture, const render_vertex* v0, const render_vertex* v1, const render_vertex* v2, const render_vertex* v3 );
	void add_triangle( texture_asset* texture, const render_vertex* v0, const render_vertex* v1, const render_vertex* v2 );
	void add_line( texture_asset* texture, const render_vertex* v0, const render_vertex* v1 );
	void add_point( texture_asset* texture, const render_vertex* v0 );
};

}
