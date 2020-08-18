#pragma once

struct w_cache_assets
{
	std::map<std::string, std::unique_ptr<i_asset>> cache;

	i_asset* add( std::unique_ptr< i_asset> asset, const std::string_view name, const std::string_view filename );
	i_asset* find( const std::string_view name, bool silent = false );
};
