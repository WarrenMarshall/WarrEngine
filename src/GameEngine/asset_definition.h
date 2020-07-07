
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

// #todo - does this class need to exist?
struct w_asset_definition
{
	w_keyvalues kv;
};
