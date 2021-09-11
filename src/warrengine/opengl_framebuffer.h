
namespace war
{

struct OpenGL_Frame_Buffer
{
	std::vector<OpenGL_Color_Attachment> color_attachments;
	std::string base_name;
	uint32_t fb_id = 0;
	uint32_t rbo_id = 0;

	OpenGL_Frame_Buffer() = delete;
	OpenGL_Frame_Buffer( std::string_view base_name );
	~OpenGL_Frame_Buffer();

	void add_color_attachment( Vec2 sz, const Color& clear_color = Color::black );
	void add_depth_attachment( Vec2 sz );
	void finalize();
	void bind();
	void unbind();
	void clear_color_attachments();
};

}
