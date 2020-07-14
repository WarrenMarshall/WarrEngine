
#include "master_pch.h"
#include "master_header.h"

bool w_keyvalues::does_key_exist( const std::string& key ) const
{
	return (_data.count( key ) > 0);
}

std::string w_keyvalues::find_value( const std::string& key ) const
{
	try
	{
		return _data.at( key );
	}
	catch( const std::out_of_range& oor )
	{
		log_error( "%s : %s : '%s'", __FUNCTION__, oor.what(), key.c_str() );
		assert( false );
		return nullptr;
	}
}

void w_keyvalues::add( const std::string& key, const std::string& value )
{
	#if !defined(FINALRELEASE)
		assert( !does_key_exist( key ) );
	#endif

	_data.insert( std::make_pair( key, value ) );
}

std::map<std::string, std::string>* w_keyvalues::data()
{
	return &_data;
}
