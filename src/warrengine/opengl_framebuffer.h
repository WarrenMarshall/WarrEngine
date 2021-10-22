
namespace war
{

struct OpenGL_Frame_Buffer final
{
	std::vector<OpenGL_Color_Attachment> color_attachments;
	std::string base_name;
	ui32 fb_id = 0;
	ui32 rbo_id = 0;

	OpenGL_Frame_Buffer() = default;
	OpenGL_Frame_Buffer( std::string_view base_name );
	~OpenGL_Frame_Buffer();

	void init( std::string name );
	void add_color_attachment( Vec2 sz, const Color& clear_color = Color::black );
	void add_depth_attachment( Vec2 sz );
	void finalize();
	void bind();
	void unbind();
	void clear_color_attachments();
};

}
