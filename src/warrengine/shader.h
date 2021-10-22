
namespace war
{

struct Shader final
{
	ui32 gl_id = 0;

	Shader() = default;
	Shader( std::string_view vert_filename, std::string_view frag_filename );

	void create_and_compile( std::string_view vert_filename, std::string_view frag_filename );

	void bind();
	void unbind();
};

}
