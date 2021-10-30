
#include "master_pch.h"

namespace war
{

Shader::Shader( std::string_view vert_filename, std::string_view frag_filename )
{
	create_and_compile( vert_filename, frag_filename );
}

void Shader::create_and_compile( std::string_view vert_filename, std::string_view frag_filename )
{
	i32 success;
	char infoLog[ 512 ];
	const char* cptr;

	// vertex shader
	ui32 vertex_id;
	{
		auto vertex_shader_src = File_System::load_binary_file( std::format( "data/warrengine/shaders/{}", vert_filename ) );
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
	ui32 fragment_id;
	{
		auto fragment_shader_src = File_System::load_text_file_raw( std::format( "data/warrengine/shaders/{}", frag_filename ) );

		std::string pragma_include( "#pragma include " );
		for( auto& line : fragment_shader_src->lines )
		{
			// ----------------------------------------------------------------------------
			// #pragma include "filename"

			auto pos = line.find( pragma_include, 0 );
			if( pos != std::string::npos )
			{
				auto filename = line.substr( pos + pragma_include.size() );
				String_Util::erase_char( filename, '\"' );

				auto include_file = File_System::load_text_file_raw( std::format( "data/warrengine/shaders/inc/{}", filename ) );

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

void Shader::bind()
{
	glUseProgram( gl_id );
}

void Shader::unbind()
{
	glUseProgram( 0 );
}

}
