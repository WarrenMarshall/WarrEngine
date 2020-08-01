
#include "master_pch.h"
#include "master_header.h"

bool w_keyvalues::does_key_exist( const std::string_view key ) const
{
	return _kv.count( key.data() ) > 0;
}

std::string_view w_keyvalues::find_value( const std::string_view key ) const
{
	try
	{
		return std::string_view( _kv.at( key.data() ) );
	}
	catch( const std::out_of_range& oor )
	{
		log_error( fmt::format( "{} : {} : '{}'", __FUNCTION__, oor.what(), key ) );
		assert( false );
		return nullptr;
	}
}

void w_keyvalues::add( const std::string_view key, const std::string_view value )
{
	_kv.insert( std::make_pair( key, value ) );
}

std::map<std::string, std::string>* w_keyvalues::data()
{
	return &_kv;
}
