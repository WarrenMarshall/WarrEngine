#pragma once

struct w_opengl
{
	std::vector<w_matrix> modelview_stack;

	std::unique_ptr<w_shader> base_shader = nullptr;
	std::unique_ptr<w_shader> base_with_bloom_shader = nullptr;
	std::unique_ptr<w_shader> blur_shader = nullptr;

	void init();

	w_matrix* push();
	w_matrix* push_identity();
	w_matrix* top();
	w_matrix* pop();

	void clear_texture_bind() const;

	void set_blend( e_opengl_blend blend ) const;

	void init_projection_matrix() const;
	void init_view_matrix( w_camera* camera ) const;
	void init_view_matrix_identity() const;
	void init_view_matrix_identity_ui() const;
	void set_uniform( std::string_view name, float value );
	void set_uniform( std::string_view name, bool value );
	void set_uniform( std::string_view name, w_color value );
};
