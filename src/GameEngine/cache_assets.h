#pragma once

struct w_cache_assets
{
	std::unordered_map<std::string, std::unique_ptr<i_asset>> cache;

	i_asset* add( std::unique_ptr< i_asset> asset, const std::string& name, const std::string& filename );
	i_asset* find( const std::string& name, bool silent = false );
};
