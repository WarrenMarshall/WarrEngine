
#include "master_pch.h"
#include "master_header.h"

void w_logfile::init( const std::string_view name )
{
	fopen_s( &file, fmt::format( "{}_log.txt", name ).c_str(), "wt" );
	assert( file );
}

void w_logfile::deinit()
{
	fclose( file );
}

void w_logfile::_write_line( const std::string_view string_to_log )
{
	if( string_to_log.length() )
	{
		fmt::print( string_to_log.data() );
		fmt::print( logfile->file, string_to_log.data() );
	}
}

void w_logfile::time_stamp( const std::string_view msg )
{
	time_t raw_time;
	tm time_info;
	char time_str[ 100 ];

	time( &raw_time );
	localtime_s( &time_info, &raw_time );
	strftime( &time_str[ 0 ], 100, "%c", &time_info );

	log_msg( "{} : {}", msg, time_str );
}

void w_logfile::msg( const std::string_view msg )
{
	_write_line( std::string( msg ) );
}

void w_logfile::error( const std::string_view msg )
{
	_write_line( msg );

#ifdef _DEBUG
	DebugBreak();
#endif
	throw( std::exception( std::string( msg ).c_str() ) );
}
