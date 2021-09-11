
namespace war
{

struct Vertex_Buffer final
{
	uint32_t gl_id;

	// pool of contiguous vertex data
	Object_Pool<Render_Vertex> vertices;
	std::vector<const Texture_Asset*> texture_slots;

	Cache<const Texture_Asset*,size_t> cached_texture_slot;
	size_t total_texture_slots_used = 0;

	// how many vertices make up a single element.
	// i.e. quad = 4, triangles = 3, line = 2, point = 1
	int32_t verts_per_element = -1;

	Vertex_Array_Object* vao = nullptr;

	// ----------------------------------------------------------------------------

	Vertex_Buffer() = default;
	Vertex_Buffer( Vertex_Array_Object* vao, int32_t verts_per_element );
	virtual ~Vertex_Buffer();

	virtual void bind();
	virtual void unbind();
	void bind_texture_units();
	virtual void upload_vertices_to_gpu();
	virtual void reset();

	void set_up_vertex_attribs();
	size_t assign_texture_slot( const Texture_Asset* texture );
};

}
