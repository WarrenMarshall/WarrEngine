
#pragma once

struct w_logfile
{
	void init( const char* name );
	void deinit();

	void _write_line( const std::string& string_to_log );

	void msg( const std::string msg, ...  );
	void warning( const std::string msg, ...  );
	void error( const std::string msg, ...  );

private:
	FILE* file;
};
