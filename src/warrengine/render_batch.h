
namespace war
{

// ----------------------------------------------------------------------------

struct render_batch
{
	// the max per batch before we force a flush and set up a new batch.
	static const int max_elements_per_draw_call = 15'000;

	render_batch() = default;
	render_batch( render_batch& ) = delete;
	render_batch( e_render_prim render_prim );

	void init( e_render_prim render_prim );
	vertex_array_object vao;

	void draw();

	void add_quad( texture_asset* texture, const render_vertex* v0, const render_vertex* v1, const render_vertex* v2, const render_vertex* v3 );
	void add_triangle( texture_asset* texture, const render_vertex* v0, const render_vertex* v1, const render_vertex* v2 );
	void add_line( texture_asset* texture, const render_vertex* v0, const render_vertex* v1 );
	void add_point( texture_asset* texture, const render_vertex* v0 );

	bool is_empty();

private:
	virtual void add_vert( texture_asset* texture, const render_vertex* render_vert );
};

// ----------------------------------------------------------------------------

struct render_batch_group
{
	std::array<render_batch, 4> batches;

	bool invalidated = false;

	void init();
	bool is_empty();
	void flush_and_reset();
	void flush_and_reset_internal();

	void add_quad( texture_asset* texture, const render_vertex* v0, const render_vertex* v1, const render_vertex* v2, const render_vertex* v3 );
	void add_triangle( texture_asset* texture, const render_vertex* v0, const render_vertex* v1, const render_vertex* v2 );
	void add_line( texture_asset* texture, const render_vertex* v0, const render_vertex* v1 );
	void add_point( texture_asset* texture, const render_vertex* v0 );
};

}
