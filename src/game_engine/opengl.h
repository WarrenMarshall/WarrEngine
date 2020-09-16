#pragma once

struct w_opengl
{
	std::vector<w_matrix> modelview_stack;
	std::unique_ptr<w_opengl_framebuffer> fb_game = nullptr;
	std::unique_ptr<w_opengl_framebuffer> fb_ui = nullptr;

	void init();

	w_matrix* push();
	w_matrix* push_identity();
	w_matrix* top();
	w_matrix* pop();

	void clear_texture_bind() const;

	void set_blend( e_opengl_blend blend ) const;

	unsigned fbo;
};
