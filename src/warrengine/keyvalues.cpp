
#include "master_pch.h"

namespace war
{

bool Key_Values::does_key_exist( const std::string& key ) const
{
	return kv.count( key ) > 0;
}

std::string_view Key_Values::find_value( const std::string& key ) const
{
	if( !does_key_exist( key ) )
	{
		return "";
	}

	return kv.at( key );
}

// same as "find_values", but it isn't fatal if the key doesn't exist
std::string_view Key_Values::find_value_or( const std::string& key, const std::string& default_value ) const
{
	if( !does_key_exist( key ) )
	{
		return default_value;
	}

	return kv.at( key );
}

}
