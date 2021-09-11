
namespace war
{

struct Log_File final
{
	FILE* file;

	void init( std::string_view name );
	void deinit();

	void _write_line( std::string_view string_to_log );

	void time_stamp( std::string_view msg );
	void msg( std::string_view msg );
	void verbose( std::string_view msg );
	void error( std::string_view msg );
};

}
