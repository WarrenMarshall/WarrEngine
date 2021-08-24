
namespace war
{

struct vertex_buffer final
{
	unsigned int gl_id;

	// pool of contiguous vertex data
	object_pool<render_vertex> vertices;
	std::vector<const texture_asset*> texture_slots;

	cache<const texture_asset*,size_t> cached_texture_slot;
	size_t total_texture_slots_used = 0;

	// how many vertices make up a single element.
	// i.e. quad = 4, triangles = 3, line = 2, point = 1
	int verts_per_element = -1;

	vertex_array_object* vao = nullptr;

	// ----------------------------------------------------------------------------

	vertex_buffer( vertex_array_object* vao, int verts_per_element );
	~vertex_buffer();

	virtual void bind();
	virtual void unbind();
	void bind_texture_units();
	virtual void upload_vertices_to_gpu();
	virtual void reset();

	void set_up_vertex_attribs();
	size_t assign_texture_slot( const texture_asset* texture );
};

}
