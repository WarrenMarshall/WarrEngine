
#include "master_pch.h"
#include "master_header.h"

namespace war
{

shader::shader( std::string_view vert_filename, std::string_view frag_filename )
{
	create_and_compile( vert_filename, frag_filename );
}

void shader::create_and_compile( std::string_view vert_filename, std::string_view frag_filename )
{
	int success;
	char infoLog[ 512 ];
	const char* cptr;

	// vertex shader
	unsigned int vertex_id;
	{
		auto vertex_shader_src = file_system::load_binary_file( std::format( "data/warr_engine/shaders/{}", vert_filename ) );
		auto wk = std::string( vertex_shader_src->buffer.begin(), vertex_shader_src->buffer.end() );

		vertex_id = glCreateShader( GL_VERTEX_SHADER );
		cptr = wk.c_str();
		glShaderSource( vertex_id, 1, &cptr, nullptr );
		glCompileShader( vertex_id );

		glGetShaderiv( vertex_id, GL_COMPILE_STATUS, &success );
		if( !success )
		{
			glGetShaderInfoLog( vertex_id, 512, nullptr, infoLog );
			log_fatal( "Vertex Shader : \"{}\" : {}", vert_filename, infoLog );
		}
	}

	// fragment shader
	std::string wk;
	unsigned int fragment_id;
	{
		auto fragment_shader_src = file_system::load_text_file_raw( std::format( "data/warr_engine/shaders/{}", frag_filename ) );

		std::string pragma_include( "#pragma include " );
		for( auto& line : fragment_shader_src->lines )
		{
			// ----------------------------------------------------------------------------
			// #pragma include "filename"

			auto pos = line.find( pragma_include, 0 );
			if( pos != std::string::npos )
			{
				auto filename = line.substr( pos + pragma_include.size() );
				string_util::erase_char( filename, '\"' );

				auto include_file = file_system::load_text_file_raw( std::format( "data/warr_engine/shaders/inc/{}", filename ) );

				for( auto& inc_line : include_file->lines )
				{
					wk += inc_line;
					wk += "\n";
				}

				continue;

			}

			wk += line;
			wk += "\n";
		}

		fragment_id = glCreateShader( GL_FRAGMENT_SHADER );
		cptr = wk.c_str();
		glShaderSource( fragment_id, 1, &cptr, nullptr );
		glCompileShader( fragment_id );

		glGetShaderiv( fragment_id, GL_COMPILE_STATUS, &success );
		if( !success )
		{
			glGetShaderInfoLog( fragment_id, 512, nullptr, infoLog );
			log_fatal( "Fragment Shader : \"{}\" : {}", frag_filename, infoLog );
		}
	}

	// shader program
	{
		gl_id = glCreateProgram();

		glAttachShader( gl_id, vertex_id );
		glAttachShader( gl_id, fragment_id );
		glLinkProgram( gl_id );

		glGetProgramiv( gl_id, GL_LINK_STATUS, &success );
		if( !success )
		{
			glGetProgramInfoLog( gl_id, 512, nullptr, infoLog );
			log_fatal( "Shader Program : {}", infoLog );
		}

		glValidateProgram( gl_id );
	}

	// clean up
	glDeleteShader( vertex_id );
	glDeleteShader( fragment_id );
}

void shader::bind()
{
	glUseProgram( gl_id );
}

void shader::unbind()
{
	glUseProgram( 0 );
}

}
