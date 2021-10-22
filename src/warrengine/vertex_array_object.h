
namespace war
{

struct Vertex_Array_Object final
{
	e_render_prim render_prim = e_render_prim::quad;
	ui32 gl_id = 0;
	Primitive_Batch* batch = nullptr;

	f32 indices_to_verts_factor = 0.f;

	std::unique_ptr<Vertex_Buffer> vb = nullptr;
	Index_Buffer* ib = nullptr;

	Vertex_Array_Object() = default;
	Vertex_Array_Object( Vertex_Array_Object& ) = delete;
	Vertex_Array_Object( Primitive_Batch* batch, e_render_prim render_prim );
	void init( Primitive_Batch* batch, e_render_prim render_prim );
	~Vertex_Array_Object();

	void bind();
	void unbind();
	void update_stats();
	void upload_vertices_to_gpu();
	void flush_and_reset( e_draw_call draw_call );
	void reset();
	GLenum get_gl_prim_type();
	void draw( e_draw_call draw_call );
};

}
