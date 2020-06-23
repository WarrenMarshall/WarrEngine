
#pragma once

// ----------------------------------------------------------------------------
// a chunk read from an asset definition file.
// chunks are surrounded by curly braces and contain a list of key/values.
//
// required key values : "type" and "name"
//
// {
//		"type" "value"
//		"name" "value"
//		"key" "value"
//		.
//		.
//		"key" "value"
// }

struct w_asset_definition
{
	std::unordered_map<std::string, std::string> key_values;
};
