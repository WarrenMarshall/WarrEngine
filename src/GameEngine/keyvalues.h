#pragma once

struct w_keyvalues
{
	bool does_key_exist( const std::string& key ) const;
	std::string find_value( const std::string& key ) const;
	void add( const std::string& key, const std::string& value );
	std::map<std::string, std::string>* data();

private:
	std::map<std::string, std::string> _kv;
};
