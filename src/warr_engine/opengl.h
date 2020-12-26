#pragma once

struct w_opengl
{
	std::vector<w_matrix> modelview_stack;

	std::unordered_map<std::string, std::unique_ptr<w_shader>> shaders;

	GLint max_texture_image_units = -1;
	std::vector<int> texture_slots;
	void init();

	w_matrix* push();
	w_matrix* push_identity();
	w_matrix* top();
	w_matrix* pop();

	void set_blend( e_opengl_blend blend ) const;

	void init_projection_matrix() const;
	void init_view_matrix( e_camera* camera ) const;
	void init_view_matrix_identity() const;
	void init_view_matrix_identity_ui() const;
	void set_uniform( std::string_view name, float value );
	void set_uniform( std::string_view name, bool value );
	void set_uniform( std::string_view name, w_color value );
	void set_uniform_array( std::string_view name, int* value, int count );
};
