#pragma once

struct w_opengl_framebuffer
{
	unsigned fb_id = 0;
	a_texture* tex = nullptr;
	unsigned rbo = 0;

	w_opengl_framebuffer();
	~w_opengl_framebuffer();

	void bind();
	void unbind();
};

