#pragma once

struct w_cache_assets
{
	std::map<std::string, std::unique_ptr<i_asset>> cache;

	i_asset* add( std::unique_ptr< i_asset> asset, const char* name, const char* filename );
	i_asset* find( const char* name, bool silent = false );
};
