#pragma once

struct w_opengl_color_attachment
{
	w_opengl_color_attachment( w_opengl_framebuffer* fb_owner );

	w_opengl_framebuffer* fb_owner = nullptr;
	a_texture* texture = nullptr;
};

// ----------------------------------------------------------------------------

struct w_opengl_framebuffer
{
	std::vector<w_opengl_color_attachment> color_attachments;
	std::string base_name;
	unsigned fb_id = 0;

	w_opengl_framebuffer() = delete;
	w_opengl_framebuffer( const std::string& base_name );
	~w_opengl_framebuffer();

	void add_color_attachment();
	void finalize();
	void bind();
	void unbind();
};

