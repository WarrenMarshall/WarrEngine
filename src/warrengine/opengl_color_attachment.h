
namespace war
{

struct OpenGL_Color_Attachment
{
	OpenGL_Color_Attachment( OpenGL_Frame_Buffer* fb_owner, Vec2 sz, const Color& clear_color );

	void clear();

	OpenGL_Frame_Buffer* fb_owner = nullptr;
	Texture_Asset* texture = nullptr;
	Color clear_color = Color::black;
};

}
