#include "master_pch.h"
#include "master_header.h"

w_shader::w_shader( const std::string_view vert_filename, const std::string_view frag_filename )
{
    create_and_compile( vert_filename, frag_filename );
}

void w_shader::create_and_compile( const std::string_view vert_filename, const std::string_view frag_filename )
{
    int  success;
    char infoLog[512];
    const char* cptr;
    std::string wk;

    // vertex shader
    unsigned int vertex_id;
    {
        auto vertex_shader_src = engine->fs->load_file_into_memory( fmt::format( "data/warr_engine/shaders/{}", vert_filename ) );
        vertex_id = glCreateShader( GL_VERTEX_SHADER );
        wk = std::string( vertex_shader_src->buffer->begin(), vertex_shader_src->buffer->end() );
        cptr = wk.c_str();
        glShaderSource( vertex_id, 1, &cptr, nullptr );
        glCompileShader( vertex_id );

        glGetShaderiv( vertex_id, GL_COMPILE_STATUS, &success );
        if( !success )
        {
            glGetShaderInfoLog( vertex_id, 512, nullptr, infoLog );
            log_error( "Vertex Shader : {}", infoLog );
        }
    }

    // fragment shader
    unsigned int fragment_id;
    {
        auto fragment_shader_src = engine->fs->load_file_into_memory( fmt::format( "data/warr_engine/shaders/{}", frag_filename ) );
        fragment_id = glCreateShader( GL_FRAGMENT_SHADER );
        wk = std::string( fragment_shader_src->buffer->begin(), fragment_shader_src->buffer->end() );
        cptr = wk.c_str();
        glShaderSource( fragment_id, 1, &cptr, nullptr );
        glCompileShader( fragment_id );

        glGetShaderiv( fragment_id, GL_COMPILE_STATUS, &success );
        if( !success )
        {
            glGetShaderInfoLog( fragment_id, 512, nullptr, infoLog );
            log_error( "Fragment Shader : {}", infoLog );
        }
	}

    // shader program
    {
        id = glCreateProgram();

        glAttachShader( id, vertex_id );
        glAttachShader( id, fragment_id );
        glLinkProgram( id );

        glGetProgramiv( id, GL_LINK_STATUS, &success );
        if( !success )
        {
            glGetProgramInfoLog( id, 512, nullptr, infoLog );
            log_error( "Shader Program : {}", infoLog );
        }

        glValidateProgram( id );
	}

    // clean up
    glDeleteShader( vertex_id );
    glDeleteShader( fragment_id );
}

void w_shader::bind()
{
    glUseProgram( id );
}

void w_shader::unbind()
{
    glUseProgram( 0 );
}
