#pragma once

struct w_keyvalues
{
	_NODISCARD bool does_key_exist( const std::string_view key ) const;
	_NODISCARD std::string_view find_value( const std::string& key ) const;
	_NODISCARD std::string_view find_value_opt( const std::string& key, const std::string& default_value = "" ) const;
	void insert_or_assign( const std::string_view key, const std::string_view value );

	std::unordered_map<std::string, std::string> kv;
};
