
namespace war
{

// a single asset definition file, broken up into it's chunks
//
// a chunk is a set of key/values that are grouped together to form
// an asset definition, scoped between curly braces.

struct asset_file_definition
{
	std::string original_filename;
	std::vector<key_values> asset_definitions;

	std::string data_folder;

	void precache_asset_resources( size_t pass_num );
	void precache_preproc( const key_values& key_values_for_asset_def );
	void precache_palette( const key_values& key_values_for_asset_def, std::string_view tag );
	void precache_src_texture( const key_values& key_values_for_asset_def, std::string_view tag );
	void precache_src_gradient( const key_values& key_values_for_asset_def, std::string_view tag );
	void precache_font_def( const key_values& key_values_for_asset_def, std::string_view tag );
	void precache_slice_def( const key_values& key_values_for_asset_def, std::string_view tag );
	void precache_sound( const key_values& key_values_for_asset_def, std::string_view tag );
	void precache_anim_texture( const key_values& key_values_for_asset_def, std::string_view tag );
	void precache_texture( const key_values& key_values_for_asset_def, std::string_view tag );
	void precache_emitter_params( const key_values& key_values_for_asset_def, std::string_view tag );
	void precache_font( const key_values& key_values_for_asset_def, std::string_view tag );
	void precache_mesh( const key_values& key_values_for_asset_def, std::string_view tag );
	void precache_cursor( const key_values& key_values_for_asset_def, std::string_view tag );
	void precache_tile_set( const key_values& key_values_for_asset_def, std::string_view tag );
	void precache_tile_map( const key_values& key_values_for_asset_def, std::string_view tag );

	bool create_internals();
};

}
