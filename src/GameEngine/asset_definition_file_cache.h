#pragma once

struct w_cache_asset_definition_files
{
	std::map<std::string, std::unique_ptr<w_asset_definition_file>> cache;

	w_asset_definition_file* add( const char* name, const char* filename );
	w_asset_definition_file* find( const char* name, bool silent = false );
};
