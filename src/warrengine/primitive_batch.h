
namespace war
{

// ----------------------------------------------------------------------------

struct Primitive_Batch final
{
	// the max per batch before we force a flush and set up a new batch.
	static const int max_elements_per_draw_call = 15'000;

	Primitive_Batch() = default;
	Primitive_Batch( Primitive_Batch& ) = delete;
	Primitive_Batch( e_render_prim render_prim );

	void init( e_render_prim render_prim );
	Vertex_Array_Object vao[ draw_call::max ];

	void add_quad( Texture_Asset* texture, const Render_Vertex* v0, const Render_Vertex* v1, const Render_Vertex* v2, const Render_Vertex* v3 );
	void add_triangle( Texture_Asset* texture, const Render_Vertex* v0, const Render_Vertex* v1, const Render_Vertex* v2 );
	void add_line( Texture_Asset* texture, const Render_Vertex* v0, const Render_Vertex* v1 );
	void add_point( Texture_Asset* texture, const Render_Vertex* v0 );

	bool is_empty();

private:
	virtual void add_vert( e_draw_call draw_call, Texture_Asset* texture, const Render_Vertex* render_vert );
};

// ----------------------------------------------------------------------------

struct Primitive_Batch_Group final
{
	std::array<Primitive_Batch, 4> batches;

	bool invalidated = false;

	void init();
	bool is_empty();
	void flush_and_reset( e_draw_call draw_call );
	void flush_and_reset_internal( e_draw_call draw_call );

	void add_quad( Texture_Asset* texture, const Render_Vertex* v0, const Render_Vertex* v1, const Render_Vertex* v2, const Render_Vertex* v3 );
	void add_triangle( Texture_Asset* texture, const Render_Vertex* v0, const Render_Vertex* v1, const Render_Vertex* v2 );
	void add_line( Texture_Asset* texture, const Render_Vertex* v0, const Render_Vertex* v1 );
	void add_point( Texture_Asset* texture, const Render_Vertex* v0 );
	size_t assign_texture_slot_manual( Texture_Asset* texture );
};

}
