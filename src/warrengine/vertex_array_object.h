
namespace war
{

struct Vertex_Array_Object final
{
	e_render_prim_t render_prim = e_render_prim::quad;
	uint32_t gl_id = 0;
	Primitive_Batch* batch = nullptr;

	float_t indices_to_verts_factor = 0.f;

	Vertex_Buffer vb;
	Index_Buffer* ib = nullptr;

	Vertex_Array_Object() = default;
	Vertex_Array_Object( Vertex_Array_Object& ) = delete;
	Vertex_Array_Object( Primitive_Batch* batch, e_render_prim_t render_prim );
	void init( Primitive_Batch* batch, e_render_prim_t render_prim );
	~Vertex_Array_Object();

	void bind();
	void unbind();
	void update_stats();
	void flush_and_reset( e_draw_call_t draw_call );
	void upload_vertices_to_gpu();
	void flush_and_reset_internal( e_draw_call_t draw_call );
	void reset();
	GLenum get_gl_prim_type();
	void draw( e_draw_call_t draw_call );
};

}
