
#include "master_pch.h"
#include "master_header.h"

void w_logfile::init( const std::string& name )
{
	fopen_s( &file, (name + "_log.txt").c_str(), "wt" );
	assert( file );
}

void w_logfile::deinit()
{
	fclose( file );
}

void w_logfile::_write_line( const std::string& string_to_log )
{
	if( string_to_log.length() )
	{
		printf( "%s\n", string_to_log.c_str() );

		fwrite( string_to_log.c_str(), 1, string_to_log.length(), file );
		fwrite( "\n", 1, 1, file );

		// force a flush here as we want the most up-to-date log file as possible
		// if there's a crash.
		fflush( file );
	}
}

void w_logfile::time_stamp( const std::string msg )
{
	time_t raw_time;
	tm time_info;
	char time_str[ 100 ];

	time( &raw_time );
	localtime_s( &time_info, &raw_time );
	strftime( &time_str[ 0 ], 100, "%c", &time_info );

	log_msg( msg + " : " + time_str );
}

void w_logfile::msg( const std::string& msg )
{
	_write_line( msg );
}

void w_logfile::warning( const std::string& msg )
{
	_write_line( "WARNING : " + msg );
}

void w_logfile::error( const std::string& msg )
{
	throw( std::exception( ( "!! : " + msg ).c_str() ) );
}
