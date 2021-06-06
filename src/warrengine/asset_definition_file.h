
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

	std::string data_folder;

	void precache_asset_resources( size_t pass_num );
	void precache_preproc( const w_keyvalues& key_values_for_asset_def );
	void precache_palette( const w_keyvalues& key_values_for_asset_def, std::string_view tag );
	void precache_src_texture( const w_keyvalues& key_values_for_asset_def, std::string_view tag );
	void precache_gradient( const w_keyvalues& key_values_for_asset_def, std::string_view tag );
	void precache_font_def( const w_keyvalues& key_values_for_asset_def, std::string_view tag );
	void precache_slice_def( const w_keyvalues& key_values_for_asset_def, std::string_view tag );
	void precache_sound( const w_keyvalues& key_values_for_asset_def, std::string_view tag );
	void precache_anim_texture( const w_keyvalues& key_values_for_asset_def, std::string_view tag );
	void precache_texture( const w_keyvalues& key_values_for_asset_def, std::string_view tag );
	void precache_emitter_params( const w_keyvalues& key_values_for_asset_def, std::string_view tag );
	void precache_font( const w_keyvalues& key_values_for_asset_def, std::string_view tag );
	void precache_mesh( const w_keyvalues& key_values_for_asset_def, std::string_view tag );
	void precache_cursor( const w_keyvalues& key_values_for_asset_def, std::string_view tag );

	bool create_internals();
};
