
namespace war
{

struct opengl_framebuffer
{
	std::vector<opengl_color_attachment> color_attachments;
	std::string base_name;
	unsigned fb_id = 0;
	unsigned rbo_id = 0;

	opengl_framebuffer() = delete;
	opengl_framebuffer( std::string_view base_name );
	~opengl_framebuffer();

	void add_color_attachment( vec2 sz, const color& clear_color = color::black );
	void add_depth_attachment( vec2 sz );
	void finalize();
	void bind();
	void unbind();
	void clear_color_attachments();
};

}
