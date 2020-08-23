#include "master_pch.h"
#include "master_header.h"

void w_shader::create_and_compile( const std::string_view vert_filename, const std::string_view frag_filename )
{
    int  success;
    char infoLog[512];
    const char* cptr;
    std::string wk;

    // vertex shader

    auto vertex_shader_src = engine->fs->load_file_into_memory( fmt::format( "game_engine_data/shaders/{}.vert" , vert_filename ) );
    unsigned int vertex_id = glCreateShader( GL_VERTEX_SHADER );
    wk = std::string( vertex_shader_src->buffer->begin(), vertex_shader_src->buffer->end() );
    cptr = wk.c_str();
    glShaderSource( vertex_id, 1, &cptr, nullptr );
    glCompileShader( vertex_id );

    glGetShaderiv( vertex_id, GL_COMPILE_STATUS, &success );
    if( !success )
    {
        glGetShaderInfoLog( vertex_id, 512, nullptr, infoLog );
        log_error( fmt::format( "{} : Vertex Shader : {}", __FUNCTION__, infoLog ) );
    }

    // fragment shader

    auto fragment_shader_src = engine->fs->load_file_into_memory( fmt::format( "game_engine_data/shaders/{}.frag", frag_filename ) );
    unsigned int fragment_id = glCreateShader( GL_FRAGMENT_SHADER );
    wk = std::string( fragment_shader_src->buffer->begin(), fragment_shader_src->buffer->end() );
    cptr = wk.c_str();
    glShaderSource( fragment_id, 1, &cptr, nullptr );
    glCompileShader( fragment_id );

    glGetShaderiv( fragment_id, GL_COMPILE_STATUS, &success );
    if( !success )
    {
        glGetShaderInfoLog( fragment_id, 512, nullptr, infoLog );
        log_error( fmt::format( "{} : Fragment Shader : {}", __FUNCTION__, infoLog ) );
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
        log_error( fmt::format( "{} : Shader Program : {}", __FUNCTION__, infoLog ) );
    }

    glValidateProgram( id );

    glDeleteShader( vertex_id );
    glDeleteShader( fragment_id );
}

unsigned int w_shader::_compile_shader( unsigned int type, const std::string_view source )
{
    unsigned int id = glCreateShader( type );

    const char* src = source.data();
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