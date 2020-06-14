#pragma once

struct w_cache_asset_definition_files
{
	std::map<std::string, std::unique_ptr<w_asset_definition_file>> cache;

	w_asset_definition_file* add( const std::string& name, const std::string& filename );
	w_asset_definition_file* find( const std::string& name, bool silent = false );
};
