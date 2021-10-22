
namespace war
{

struct OpenGL_Mgr final
{
	// used for scaling/rotating objects around their local origins and then
	// translating them into world space
	std::vector<Matrix> model_matrix_stack;
	Matrix* top_matrix = nullptr;

	Matrix* model_matrix_push();
	Matrix* model_matrix_push_identity();
	Matrix* model_matrix_pop();

	bool using_camera = false;

	// the set of shaders we have loaded
	std::map<std::string, Shader> shaders;

	// this value is polled from opengl at initialization. this allows the code
	// to dynamically work with various video cards.
	i32 max_texture_image_units = -1;

	// rendering the various primitive types always uses the same layout for the
	// index buffers. this means we can generate these one time, at start up,
	// and just reference them for every dynamic/static render batch we draw.

	std::unique_ptr<Index_Buffer_Quads> ib_quads = nullptr;
	std::unique_ptr<Index_Buffer_Tris> ib_tris = nullptr;
	std::unique_ptr<Index_Buffer_Lines> ib_lines = nullptr;
	std::unique_ptr<Index_Buffer_Points> ib_points = nullptr;

	void init();
	void clear_depth_buffer();
	void set_blend( e_opengl_blend blend ) const;

	void set_projection_matrix() const;
	void set_view_matrix_identity();
	void set_view_matrix_identity_no_camera();
	void set_view_matrix_identity_ui();

	f32 get_uniform_float( std::string_view name );
	bool get_uniform_bool( std::string_view name );
	Color get_uniform_color( std::string_view name );

	void set_uniform_float( std::string_view name, f32 value );
	void set_uniform_bool( std::string_view name, bool value );
	void set_uniform_color( std::string_view name, const Color& value );
	void set_uniform_array( std::string_view name, i32* value, i32 count );

	void allocate_vertex_buffer_on_gpu( i32 max_verts, bool is_static );
};

}
