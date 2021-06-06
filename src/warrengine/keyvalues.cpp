
#include "master_pch.h"
#include "master_header.h"

namespace war
{

bool key_values::does_key_exist( std::string_view key ) const
{
	return kv.count( key.data() ) > 0;
}

std::string_view key_values::find_value( std::string_view key ) const
{
	if( !does_key_exist( key ) )
	{
		return "";
	}

	return kv.at( key.data() );
}

// same as "find_values", but it isn't fatal if the key doesn't exist
std::string_view key_values::find_value_or( std::string_view key, std::string_view default_value ) const
{
	if( !does_key_exist( key ) )
	{
		return default_value;
	}

	return std::string_view( kv.at( key.data() ) );
}

}
