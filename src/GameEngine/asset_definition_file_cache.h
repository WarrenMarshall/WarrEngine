#pragma once

struct w_cache_asset_definition_files
{
	std::map<std::string, std::shared_ptr<w_asset_definition_file>> cache;

	virtual ~w_cache_asset_definition_files() = default;

	w_asset_definition_file* add( const std::string_view name, const std::string_view filename );
};
