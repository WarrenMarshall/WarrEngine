#pragma once

struct GE_API w_shader
{
    unsigned int id = 0;

    void create_and_compile( const std::string_view vert_filename, const std::string_view frag_filename );
    unsigned int _compile_shader( unsigned int type, const std::string_view source );

    void bind();
    void unbind();
};
