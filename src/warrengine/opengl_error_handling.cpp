
// ----------------------------------------------------------------------------
// callback so OpenGL can relay messages and warnings
//
// note : this code is #included into the project manually.

void GLAPIENTRY OpenGL_MessageCallback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam )
{
	auto const src_str = [source] ()
	{
		switch( source )
		{
			case GL_DEBUG_SOURCE_API: return "API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
			case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
			case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
			case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
			default: return "OTHER";
		}
	}( );

	auto const type_str = [type] ()
	{
		switch( type )
		{
			case GL_DEBUG_TYPE_ERROR: return "ERROR";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
			case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
			case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
			case GL_DEBUG_TYPE_MARKER: return "MARKER";
			default: return "OTHER";
		}
	}( );

	auto const severity_str = [severity] ()
	{
		switch( severity )
		{
			case GL_DEBUG_SEVERITY_LOW: return "LOW";
			case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
			case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
			default: return "NOTIFICATION";
		}
	}( );

	if( type == GL_DEBUG_TYPE_ERROR )
	{
		log_fatal( "{}: {}: {}: {}", src_str, type_str, severity_str, message );
	}
	else
	{
		log_verbose( "{}: {}: {}: {}", src_str, type_str, severity_str, message );
	}
}

