#pragma once

struct w_cache_asset_definition_files
{
	// a vector because we want to store these files in the order we find them on the hard drive
	std::vector<w_asset_definition_file> cache;

	void add( const std::string_view filename );
};
