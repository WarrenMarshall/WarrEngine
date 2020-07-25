#include "master_pch.h"
#include "master_header.h"

void w_shader::create_and_compile( const std::string& vert_filename, const std::string& frag_filename )
{
    int  success;
    char infoLog[512];
    const char* cptr;
    std::string wk;

    // vertex shader

    auto vertex_shader_src = engine->fs->load_file_into_memory( "shaders/" + vert_filename + ".vert" );
    unsigned int vertex_id = glCreateShader( GL_VERTEX_SHADER );
    wk = std::string( vertex_shader_src->buffer->begin(), vertex_shader_src->buffer->end() );
    cptr = wk.c_str();
    glShaderSource( vertex_id, 1, &cptr, nullptr );
    glCompileShader( vertex_id );

    glGetShaderiv( vertex_id, GL_COMPILE_STATUS, &success );
    if( !success )
    {
        glGetShaderInfoLog( vertex_id, 512, nullptr, infoLog );
        log_error( "%s : Vertex Shader : %s", __FUNCTION__, infoLog );
    }

    // fragment shader

    auto fragment_shader_src = engine->fs->load_file_into_memory( "shaders/" + frag_filename + ".frag" );
    unsigned int fragment_id = glCreateShader( GL_FRAGMENT_SHADER );
    wk = std::string( fragment_shader_src->buffer->begin(), fragment_shader_src->buffer->end() );
    cptr = wk.c_str();
    glShaderSource( fragment_id, 1, &cptr, nullptr );
    glCompileShader( fragment_id );

    glGetShaderiv( fragment_id, GL_COMPILE_STATUS, &success );
    if( !success )
    {
        glGetShaderInfoLog( fragment_id, 512, nullptr, infoLog );
        log_error( "%s : Fragment Shader :  %s", __FUNCTION__, infoLog );
    }

    // shader program

    id = glCreateProgram();

    glAttachShader( id, vertex_id );
    glAttachShader( id, fragment_id );
    glLinkProgram( id );

    glGetProgramiv( id, GL_LINK_STATUS, &success );
    if( !success )
    {
        glGetProgramInfoLog( id, 512, nullptr, infoLog );
        log_error( "%s : Shader Program :  %s", __FUNCTION__, infoLog );
    }

    glValidateProgram( id );

    glDeleteShader( vertex_id );
    glDeleteShader( fragment_id );

	vertex_id = 0;
	fragment_id = 0;
}

unsigned int w_shader::_compile_shader( unsigned int type, const std::string& source )
{
    unsigned int id = glCreateShader( type );

    const char* src = source.c_str();
    glShaderSource( id, 1, &src, nullptr );
    glCompileShader( id );

	return id;
}

void w_shader::bind()
{
    glUseProgram( id );
}

void w_shader::unbind()
{
    glUseProgram( 0 );
}
