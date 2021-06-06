#pragma once

struct w_shader
{
	unsigned int id = 0;

	w_shader() = default;
	w_shader( const std::string_view vert_filename, const std::string_view frag_filename );

	void create_and_compile( const std::string_view vert_filename, const std::string_view frag_filename );

	void bind();
	void unbind();
};
