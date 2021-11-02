
namespace war
{

// ----------------------------------------------------------------------------

struct Primitive_Batch final
{
	// the max primitves per batch before we force a flush and set up a new batch.

#ifdef _DEBUG
	// this is set low in debug mode so we'll know if the batching code breaks.
	static const i32 max_elements_per_draw_call = 50;
#else
	static const i32 max_elements_per_draw_call = 5'000;
#endif

	Primitive_Batch() = default;
	Primitive_Batch( Primitive_Batch& ) = delete;
	Primitive_Batch( e_render_prim render_prim );

	void init( e_render_prim render_prim );
	std::array< Vertex_Array_Object, ( i32 )e_draw_call::max > vao;

	void add_quad( const Texture_Asset* texture, Render_Vertex* v0, Render_Vertex* v1, Render_Vertex* v2, Render_Vertex* v3 );
	void add_triangle( const Texture_Asset* texture, Render_Vertex* v0, Render_Vertex* v1, Render_Vertex* v2 );
	void add_line( const Texture_Asset* texture, Render_Vertex* v0, Render_Vertex* v1 );
	void add_point( const Texture_Asset* texture, Render_Vertex* v0 );

	bool is_empty();

private:
	void add_vert( e_draw_call draw_call, const Texture_Asset* texture, Render_Vertex* render_vert );
};

// ----------------------------------------------------------------------------

struct Primitive_Batch_Group final
{
	std::array<Primitive_Batch, 4> batches;

	bool invalidated = false;

	void init();
	bool is_empty();
	void flush_and_reset( e_draw_call draw_call );

	void add_quad( const Texture_Asset* texture, Render_Vertex* v0, Render_Vertex* v1, Render_Vertex* v2, Render_Vertex* v3 );
	void add_triangle( const Texture_Asset* texture, Render_Vertex* v0, Render_Vertex* v1, Render_Vertex* v2 );
	void add_line( const Texture_Asset* texture, Render_Vertex* v0, Render_Vertex* v1 );
	void add_point( const Texture_Asset* texture, Render_Vertex* v0 );
	size_t assign_texture_slot_manual( const Texture_Asset* texture );
};

}
