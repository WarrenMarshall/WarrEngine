#pragma once

struct w_opengl_color_attachment
{
	w_opengl_color_attachment( w_opengl_framebuffer* fb_owner, const w_color& clear_color );

	void clear();

	w_opengl_framebuffer* fb_owner = nullptr;
	a_texture* texture = nullptr;
	w_color clear_color = w_color::black;
};
