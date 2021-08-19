
namespace war
{

struct opengl
{
	// used for scaling/rotating objects around their local origins and then
	// translating them into world space
	std::vector<matrix> model_matrix_stack;
	matrix* top_matrix = nullptr;

	matrix* model_matrix_push();
	matrix* model_matrix_push_identity();
	matrix* model_matrix_pop();

	bool using_camera = false;

	// the set of shaders we have loaded
	std::unordered_map<std::string, shader> shaders;

	// this value is polled from opengl at initialization. this allows the code
	// to dynamically work with various video cards.
	GLint max_texture_image_units = -1;

	// rendering the various primitive types always uses the same layout for the
	// index buffers. this means we can generate these one time, at start up,
	// and just reference them for every dynamic/static render batch we draw.

	std::unique_ptr<index_buffer_quads> ib_quads = nullptr;
	std::unique_ptr<index_buffer_tris> ib_tris = nullptr;
	std::unique_ptr<index_buffer_lines> ib_lines = nullptr;
	std::unique_ptr<index_buffer_points> ib_points = nullptr;

	void init();
	void clear_depth_buffer();
	void set_blend( e_opengl_blend blend ) const;

	void set_projection_matrix() const;
	void set_view_matrix_identity();
	void set_view_matrix_identity_no_camera();
	void set_view_matrix_identity_ui();

	float get_uniform_float( std::string_view name );
	bool get_uniform_bool( std::string_view name );
	color get_uniform_color( std::string_view name );

	void set_uniform( std::string_view name, float value );
	void set_uniform( std::string_view name, bool value );
	void set_uniform( std::string_view name, const color& value );
	void set_uniform_array( std::string_view name, int* value, int count );

	void allocate_vertex_buffer_on_gpu( int max_verts, bool is_static );
};

}
