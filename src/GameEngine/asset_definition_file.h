
// ----------------------------------------------------------------------------
// a single asset definition file, broken up into it's chunks
//
// a chunk is a set of key/values that are grouped together to form
// an asset definition, scoped between curly braces.

struct w_asset_definition_file : i_reloadable
{
	std::string name = "";
	std::vector<std::unique_ptr<w_keyvalues>> asset_definitions;

	~w_asset_definition_file();

	void precache_asset_resources( size_t pass_num, bool is_hot_reloading );

	void clean_up_internals();
	bool create_internals( bool is_hot_reloading );
};
