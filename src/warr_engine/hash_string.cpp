
#include "master_pch.h"
#include "master_header.h"

std::hash<std::string> w_hash::string_hasher = std::hash<std::string>();

w_hash::w_hash()
{
	tag = 0;
}

w_hash::w_hash( const std::string& str )
{
	tag = w_hash::string_hasher( str );
}
