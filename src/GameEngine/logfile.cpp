
#include "master_pch.h"
#include "master_header.h"

void w_logfile::init( const char* name )
{
	fopen_s( &file, s_format( "%s_log.txt", name ).c_str(), "wt" );
}

void w_logfile::deinit()
{
	fclose( file );
}

void w_logfile::_write_line( const std::string& string_to_log )
{
	if( string_to_log.length() )
	{
		printf( string_to_log.c_str() );
		printf( "\n" );

		fwrite( string_to_log.c_str(), 1, string_to_log.length(), file );
		fwrite( "\n", 1, 1, file );

		// force a flush here as we want the most up-to-date log file as possible
		// if there's a crash.
		fflush( file );
	}
}

void w_logfile::msg( const std::string msg, ... )
{
	va_list vargs;
	va_start( vargs, msg );
	std::string str = w_stringutil::format_valist( msg, vargs );
	va_end( vargs );

	_write_line( str );
}

void w_logfile::warning( const std::string msg, ... )
{
	va_list vargs;
	va_start( vargs, msg );
	std::string str = w_stringutil::format_valist( "WARNING : " + msg, vargs );
	va_end( vargs );

	_write_line( str );
}

void w_logfile::error( const std::string msg, ... )
{
	va_list vargs;
	va_start( vargs, msg );
	std::string str = w_stringutil::format_valist( "!! : " + msg, vargs );
	va_end( vargs );

	throw( std::exception( str.c_str() ) );
}
