
#include "master_pch.h"
#include "master_header.h"

bool w_keyvalues::does_key_exist( const std::string_view key ) const
{
	return kv.count( key.data() ) > 0;
}

std::string_view w_keyvalues::find_value( const std::string& key ) const
{
	try
	{
		return std::string_view( kv.at( key ) );
	}
	catch( const std::out_of_range& oor )
	{
		log_error( "Out of range : '{}'", oor.what(), key );
		return nullptr;
	}
}

// same as "find_values", but it isn't fatal if the key doesn't exist
std::string_view w_keyvalues::find_value_opt( const std::string& key, const std::string& default_value ) const
{
	if( !does_key_exist( key ) )
	{
		return default_value;
	}

	return std::string_view( kv.at( key ) );
}

void w_keyvalues::set( const std::string_view key, const std::string_view value )
{
	kv.insert_or_assign( std::string( key ), std::string( value ) );
}
