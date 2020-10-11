#pragma once

struct w_opengl_framebuffer
{
	a_texture* tex = nullptr;
	float w, h;
	unsigned fb_id = 0;
	unsigned rbo = 0;

	w_opengl_framebuffer() = delete;
	w_opengl_framebuffer( const std::string& base_name, float w, float h );
	~w_opengl_framebuffer();

	void bind();
	void unbind();
};

