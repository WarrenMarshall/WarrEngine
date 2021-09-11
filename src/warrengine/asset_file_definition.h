
namespace war
{

// a single asset definition file, broken up into it's chunks
//
// a chunk is a set of key/values that are grouped together to form
// an asset definition, scoped between curly braces.

struct Asset_File_Definition
{
	std::string original_filename;
	std::vector<Key_Values> asset_definitions;

	std::string data_folder;

	void precache_asset_resources( size_t pass_num );
	void precache_preproc( const Key_Values& key_values_for_asset_def );
	void precache_palette( const Key_Values& key_values_for_asset_def, std::string_view tag );
	void precache_src_texture( const Key_Values& key_values_for_asset_def, std::string_view tag );
	void precache_src_gradient( const Key_Values& key_values_for_asset_def, std::string_view tag );
	void precache_font_def( const Key_Values& key_values_for_asset_def, std::string_view tag );
	void precache_slice_def( const Key_Values& key_values_for_asset_def, std::string_view tag );
	void precache_sound( const Key_Values& key_values_for_asset_def, std::string_view tag );
	void precache_anim_texture( const Key_Values& key_values_for_asset_def, std::string_view tag );
	void precache_texture( const Key_Values& key_values_for_asset_def, std::string_view tag );
	void precache_emitter_params( const Key_Values& key_values_for_asset_def, std::string_view tag );
	void precache_font( const Key_Values& key_values_for_asset_def, std::string_view tag );
	void precache_mesh( const Key_Values& key_values_for_asset_def, std::string_view tag );
	void precache_cursor( const Key_Values& key_values_for_asset_def, std::string_view tag );
	void precache_tile_set( const Key_Values& key_values_for_asset_def, std::string_view tag );
	void precache_tile_map( const Key_Values& key_values_for_asset_def, std::string_view tag );

	bool create_internals();
};

}
