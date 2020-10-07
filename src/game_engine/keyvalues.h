#pragma once

struct w_keyvalues
{
	[[nodiscard]] bool does_key_exist( const std::string_view key ) const;
	[[nodiscard]] std::string_view find_value( const std::string& key ) const;
	[[nodiscard]] std::string_view find_value_opt( const std::string& key, const std::string& default_value = "" ) const;
	void set( const std::string_view key, const std::string_view value );

	std::map<std::string, std::string> kv;
};
