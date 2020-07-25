#pragma once

struct w_shader
{
    unsigned int id = 0;

    void create_and_compile( const std::string& vert_filename, const std::string& frag_filename );
    unsigned int _compile_shader( unsigned int type, const std::string& source );

    void bind();
    void unbind();
};
