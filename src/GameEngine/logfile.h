
#pragma once

struct GE_API w_logfile
{
	void init( const std::string_view name );
	void deinit();

	void _write_line( const std::string_view string_to_log );

	void time_stamp( const std::string_view msg );
	void msg( const std::string_view msg );
	void warning( const std::string_view msg );
	void error( const std::string_view msg );

private:
	FILE* file;
};
