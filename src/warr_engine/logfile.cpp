
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void log_file::init( std::string_view name )
{
	fopen_s( &file, std::format( "{}_log.txt", name ).c_str(), "wt" );
	assert( file );
}

void log_file::deinit()
{
	fclose( file );
}

void log_file::_write_line( std::string_view string_to_log )
{
	if( string_to_log.length() )
	{
		std::printf( string_to_log.data() );
		fwrite( string_to_log.data(), sizeof( char ), string_to_log.length(), file );
	}
}

void log_file::time_stamp( std::string_view msg )
{
	time_t raw_time;
	tm time_info;
	char time_str[ 100 ] = {};

	time( &raw_time );
	localtime_s( &time_info, &raw_time );
	strftime( &time_str[ 0 ], 100, "%c", &time_info );

	log( "{} : {}", msg, time_str );
}

void log_file::msg( std::string_view msg )
{
	_write_line( std::string( msg ) );
}

void log_file::verbose( std::string_view msg )
{
	if( g_engine->cmdline.verbose )
	{
		_write_line( std::string( msg ) );
	}
}

void log_file::error( std::string_view msg )
{
	_write_line( msg );

	// make sure the log file is written before breaking on the error
	fflush( file );
	deinit();

	// this is more useful in debug mode since it drops you right on this line
	// and you can use the callstack to walk back to the problem code.
	DebugBreak();

}

}
