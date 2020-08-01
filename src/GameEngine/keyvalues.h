#pragma once

struct w_keyvalues
{
	bool does_key_exist( const std::string_view key ) const;
	std::string_view find_value( const std::string_view key ) const;
	void add( const std::string_view key, const std::string_view value );

	std::map<std::string, std::string> kv;
};
