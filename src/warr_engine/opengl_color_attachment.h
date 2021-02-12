#pragma once

struct w_opengl_color_attachment
{
	w_opengl_color_attachment( w_opengl_framebuffer* fb_owner );

	w_opengl_framebuffer* fb_owner = nullptr;
	a_texture* texture = nullptr;
};
