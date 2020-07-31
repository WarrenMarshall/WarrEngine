
#pragma once

struct w_logfile
{
	void init( const std::string& name );
	void deinit();

	void _write_line( const std::string& string_to_log );

	void time_stamp( const std::string msg );
	void msg( const std::string& msg );
	void warning( const std::string& msg );
	void error( const std::string& msg );

private:
	FILE* file;
};
