
namespace war
{

struct vertex_array_object final
{
	e_render_prim render_prim = render_prim::quad;
	GLuint gl_id = 0;
	primitive_batch* batch = nullptr;

	float indices_to_verts_factor = 0.f;

	std::unique_ptr<vertex_buffer> vb = nullptr;
	index_buffer* ib = nullptr;

	vertex_array_object() = default;
	vertex_array_object( vertex_array_object& ) = delete;
	vertex_array_object( primitive_batch* batch, e_render_prim render_prim );
	void init( primitive_batch* batch, e_render_prim render_prim );
	~vertex_array_object();

	void bind();
	void unbind();
	void update_stats();
	void flush_and_reset( e_draw_call draw_call );
	void upload_vertices_to_gpu();
	void flush_and_reset_internal( e_draw_call draw_call );
	void reset();
	GLenum get_gl_prim_type();
	void draw( e_draw_call draw_call );
};

}
