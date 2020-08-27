
#include "master_pch.h"
#include "master_header.h"

bool w_keyvalues::does_key_exist( const std::string_view key ) const
{
	return kv.count( key.data() ) > 0;
}

std::string_view w_keyvalues::find_value( const std::string_view key ) const
{
	try
	{
		return std::string_view( kv.at( key.data() ) );
	}
	catch( const std::out_of_range& oor )
	{
		log_error( fmt::format( "{} : '{}'", __FUNCTION__, oor.what(), key ) );
		return nullptr;
	}
}

// same as "find_values", but it isn't fatal if the key doesn't exist
std::string_view w_keyvalues::find_value_opt( const std::string_view key, const std::string_view default_value ) const
{
	try
	{
		return std::string_view( kv.at( key.data() ) );
	}
	catch( const std::out_of_range& oor )
	{
		(void) oor;
		return default_value;
	}
}

void w_keyvalues::set( const std::string_view key, const std::string_view value )
{
	kv.erase( std::string( key ) );
	kv.insert( std::make_pair( key, value ) );
}
