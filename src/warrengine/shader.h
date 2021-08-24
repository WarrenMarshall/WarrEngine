
namespace war
{

struct shader final
{
	unsigned int gl_id = 0;

	shader() = default;
	shader( std::string_view vert_filename, std::string_view frag_filename );

	void create_and_compile( std::string_view vert_filename, std::string_view frag_filename );

	void bind();
	void unbind();
};

}
