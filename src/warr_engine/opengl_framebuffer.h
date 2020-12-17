#pragma once

struct w_opengl_framebuffer
{
	std::vector<a_texture*> textures;
	std::string base_name;
	float w, h;
	unsigned fb_id = 0;
	unsigned rbo_id = 0;

	w_opengl_framebuffer() = delete;
	w_opengl_framebuffer( const std::string& base_name, int num_color_attachments, float w, float h );
	~w_opengl_framebuffer();

	void bind();
	void unbind();

	void add_texture();
};

