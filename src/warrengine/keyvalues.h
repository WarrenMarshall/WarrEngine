#pragma once

struct w_keyvalues
{
	[[nodiscard]] bool does_key_exist( const std::string_view key ) const;
	[[nodiscard]] std::string_view find_value( const std::string& key ) const;
	[[nodiscard]] std::string_view find_value_opt( const std::string& key, const std::string_view default_value = "" ) const;
	void insert_or_assign( const std::string_view key, const std::string_view value );

	std::unordered_map<std::string, std::string> kv;
};
