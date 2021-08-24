
namespace war
{

struct opengl_color_attachment final
{
	opengl_color_attachment( opengl_framebuffer* fb_owner, vec2 sz, const color& clear_color );

	void clear();

	opengl_framebuffer* fb_owner = nullptr;
	texture_asset* texture = nullptr;
	color clear_color = color::black;
};

}
