
// ----------------------------------------------------------------------------
// a single asset definition file, broken up into it's chunks
//
// a chunk is a set of key/values that are grouped together to form
// an asset definition, scoped between curly braces.

struct GE_API w_asset_definition_file : i_reloadable
{
	std::vector<std::unique_ptr<w_keyvalues>> asset_definitions;

	virtual ~w_asset_definition_file() = default;

	void precache_asset_resources( size_t pass_num, bool is_hot_reloading );

	void clean_up_internals() override;
	bool create_internals( bool is_hot_reloading ) override;
};
