
// ----------------------------------------------------------------------------
// a single asset definition file, broken up into it's chunks
//
// a chunk is a set of key/values that are grouped together to form
// an asset definition, scoped between curly braces.

struct w_asset_definition_file
{
	std::string original_filename;
	std::vector<w_keyvalues> asset_definitions;

	virtual ~w_asset_definition_file() = default;

	void precache_asset_resources( size_t pass_num );

	void clean_up_internals();
	bool create_internals();
};
