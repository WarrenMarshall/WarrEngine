#pragma once

struct w_hash
{
	size_t tag;

	w_hash();
	w_hash( const std::string& str );

	bool operator==( w_hash other ) const
	{
		return tag == other.tag;
	}

	void operator=( w_hash other )
	{
		tag = other.tag;
	}

	static std::hash<std::string> string_hasher;
};
