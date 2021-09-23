
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void Asset_File_Definition::precache_asset_resources( size_t pass_num )
{
	std::string type, tag;

	std::filesystem::path filename_path = original_filename;
	data_folder = filename_path.parent_path().generic_string() + "/";

	for( const auto& key_values_for_asset_def : asset_definitions )
	{
		if( key_values_for_asset_def.kv.empty() )
		{
			continue;
		}

		type = key_values_for_asset_def.find_value( "type" );
		tag = key_values_for_asset_def.find_value( "tag" );

		switch( pass_num )
		{
			case 0:
			{
				if( type == "preproc" )				precache_preproc( key_values_for_asset_def );
				break;
			}

			case 1:
			{
				if( type == "palette" )				precache_palette( key_values_for_asset_def, tag );
				break;
			}

			case 2:
			{
				if( type == "src_texture" )			precache_src_texture( key_values_for_asset_def, tag );
				else if( type == "src_gradient" )	precache_src_gradient( key_values_for_asset_def, tag );
				else if( type == "font_def" )		precache_font_def( key_values_for_asset_def, tag );
				else if( type == "slice_def" )		precache_slice_def( key_values_for_asset_def, tag );
				else if( type == "sound" )			precache_sound( key_values_for_asset_def, tag );
				else if( type == "anim_texture" )	precache_anim_texture( key_values_for_asset_def, tag );
				else if( type == "texture" )		precache_texture( key_values_for_asset_def, tag );
				else if( type == "tile_set" )		precache_tile_set( key_values_for_asset_def, tag );
				break;
			}

			case 3:
			{
				if( type == "emitter_params" )		precache_emitter_params( key_values_for_asset_def, tag );
				else if( type == "font" )			precache_font( key_values_for_asset_def, tag );
				else if( type == "mesh" )			precache_mesh( key_values_for_asset_def, tag );
				else if( type == "cursor" )			precache_cursor( key_values_for_asset_def, tag );
				break;
			}

			case 4:
			{
				if( type == "tile_map" )			precache_tile_map( key_values_for_asset_def, tag );
				break;
			}
		}
	}
}

void Asset_File_Definition::precache_preproc( const Key_Values& key_values_for_asset_def )
{
	for( auto& [key, value] : key_values_for_asset_def.kv )
	{
		if( key != "tag" and key != "type" )
		{
			g_engine->_symbol_to_value.insert_or_assign( key, value );
		}
	}
}

void Asset_File_Definition::precache_palette( const Key_Values& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "colors" ) );

	auto asset_ptr = g_engine->asset_cache.add( std::make_unique<Palette_Asset>(), tag.data(), "" );
	asset_ptr->colors = Text_Parser::color_list_from_str( key_values_for_asset_def.find_value( "colors" ) );
	asset_ptr->create();
}

void Asset_File_Definition::precache_src_texture( const Key_Values& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "filename" ) );

	auto filename = std::format( "{}{}", data_folder, key_values_for_asset_def.find_value( "filename" ) );

	auto asset_ptr = g_engine->asset_cache.add( std::make_unique<Texture_Source_Asset>(), tag.data(), filename );
	asset_ptr->original_filename = filename;
	asset_ptr->tiling = (e_tiling)g_engine->find_int_from_symbol( key_values_for_asset_def.find_value_or( "tiling", "tiling.repeat" ) );
	asset_ptr->use_mipmaps = g_engine->find_bool_from_symbol( key_values_for_asset_def.find_value_or( " ", "true" ) );

	asset_ptr->create();

	// ----------------------------------------------------------------------------
	// every src_texture automatically creates an a_texture with the same name

	g_engine->asset_cache.add( std::make_unique<Texture_Asset>( tag ), tag.data(), "" );

	// the texture_tags" k/v is a convenient way to specify a set
	// of textures belonging to a src_texture. it's an easy way to break
	// down atlas textures or sprite sheets into individual textures.

	if( key_values_for_asset_def.does_key_exist( "texture_tags" ) )
	{
		std::string_view texture_list = key_values_for_asset_def.find_value( "texture_tags" );

		int32_t comma_count = (int32_t)( std::count( texture_list.begin(), texture_list.end(), ',' ) );

		// each texture_tag is expected to have 5 args : name,x,y,w,h
		if( ( comma_count % 5 ) != 4 )
		{
			log_fatal( "'{}' has bad formatting - too many or too few commas", texture_list );
		}

		Tokenizer tok( texture_list, "," );

		while( !tok.is_eos() )
		{
			auto texture_tag = std::string( *tok.get_next_token() );

			float_t x = Text_Parser::float_from_str( *tok.get_next_token() );
			float_t y = Text_Parser::float_from_str( *tok.get_next_token() );
			float_t w = Text_Parser::float_from_str( *tok.get_next_token() );
			float_t h = Text_Parser::float_from_str( *tok.get_next_token() );

			Rect rc( x, y, w, h );

			g_engine->asset_cache.add( std::make_unique<Texture_Asset>( tag, rc ), texture_tag, "" );
		}
	}
}

void Asset_File_Definition::precache_src_gradient( const Key_Values& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "alignment" ) );
	assert( key_values_for_asset_def.does_key_exist( "colors" ) );

	auto asset_ptr = g_engine->asset_cache.add( std::make_unique<Gradient_Source_Asset>(), tag.data(), "" );

	asset_ptr->alignment = (e_align)g_engine->find_int_from_symbol( key_values_for_asset_def.find_value( "alignment" ) );

	asset_ptr->colors.clear();
	std::vector<Color> color_list = Text_Parser::color_list_from_str( key_values_for_asset_def.find_value( "colors" ) );

	// must reverse the order or else vertical gradient textures end up
	// backwards on the screen

	if( asset_ptr->alignment == e_align::vertical )
	{
		std::reverse( color_list.begin(), color_list.end() );
	}

	for( const auto& iter : color_list )
	{
		asset_ptr->colors.push_back( iter.r );
		asset_ptr->colors.push_back( iter.g );
		asset_ptr->colors.push_back( iter.b );
		asset_ptr->colors.push_back( iter.a );
	}

	// every gradient automatically creates an a_texture with the same name

	g_engine->asset_cache.add( std::make_unique<Texture_Asset>( tag.data() ), tag.data(), "" );

	asset_ptr->create();
}

void Asset_File_Definition::precache_font_def( const Key_Values& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "filename" ) );
	assert( key_values_for_asset_def.does_key_exist( "src_texture_tag" ) );

	auto filename = std::format( "{}{}", data_folder, key_values_for_asset_def.find_value( "filename" ) );

	auto asset_ptr = g_engine->asset_cache.add( std::make_unique<Font_Def_Asset>(), tag.data(), filename );

	asset_ptr->original_filename = filename;
	asset_ptr->src_texture = g_engine->find_asset<Texture_Source_Asset>( key_values_for_asset_def.find_value( "src_texture_tag" ) );

	asset_ptr->create();
}

void Asset_File_Definition::precache_slice_def( const Key_Values& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "texture_tag" ) );
	assert( key_values_for_asset_def.does_key_exist( "x_slices" ) );
	assert( key_values_for_asset_def.does_key_exist( "y_slices" ) );

	auto asset_ptr = g_engine->asset_cache.add( std::make_unique<Slice_Def_Asset>(), tag.data(), "" );

	auto texture = g_engine->find_asset<Texture_Asset>( key_values_for_asset_def.find_value( "texture_tag" ) );
	Vec2 x_slices = Text_Parser::vec2_from_str( key_values_for_asset_def.find_value( "x_slices" ) );
	Vec2 y_slices = Text_Parser::vec2_from_str( key_values_for_asset_def.find_value( "y_slices" ) );

	float_t x, y, w, h;

	// top row

	x = texture->rc.x;
	y = texture->rc.y;
	w = x_slices.l;
	h = y_slices.t;

	auto src_texture_tag = texture->get_src_texture()->tag.data();

	asset_ptr->patches[ (int32_t)e_slice_def_patch::top_left ] =
		g_engine->asset_cache.add(
			std::make_unique<Texture_Asset>( src_texture_tag, Rect( x, y, w, h ) ), std::format( "{}_00", tag.data() ), ""
		);

	x = texture->rc.x + x_slices.l;
	w = texture->rc.w - x_slices.l - x_slices.r;
	asset_ptr->patches[ (int32_t)e_slice_def_patch::top_middle ] =
		g_engine->asset_cache.add(
			std::make_unique<Texture_Asset>( src_texture_tag, Rect( x, y, w, h ) ), std::format( "{}_10", tag.data() ), ""
		);

	x = texture->rc.x + texture->rc.w - x_slices.r;
	w = x_slices.r;
	asset_ptr->patches[ (int32_t)e_slice_def_patch::top_right ] =
		g_engine->asset_cache.add(
			std::make_unique<Texture_Asset>( src_texture_tag, Rect( x, y, w, h ) ), std::format( "{}_20", tag.data() ), ""
		);

	// middle row

	x = texture->rc.x;
	y = texture->rc.y + y_slices.t;
	w = x_slices.l;
	h = texture->rc.h - y_slices.t - y_slices.b;

	asset_ptr->patches[ (int32_t)e_slice_def_patch::middle_left ] =
		g_engine->asset_cache.add(
			std::make_unique<Texture_Asset>( src_texture_tag, Rect( x, y, w, h ) ), std::format( "{}_01", tag.data() ), ""
		);

	x = texture->rc.x + x_slices.l;
	w = texture->rc.w - x_slices.l - x_slices.r;
	asset_ptr->patches[ (int32_t)e_slice_def_patch::middle_middle ] =
		g_engine->asset_cache.add(
			std::make_unique<Texture_Asset>( src_texture_tag, Rect( x, y, w, h ) ), std::format( "{}_11", tag.data() ), ""
		);

	x = texture->rc.x + texture->rc.w - x_slices.r;
	w = x_slices.r;
	asset_ptr->patches[ (int32_t)e_slice_def_patch::middle_right ] =
		g_engine->asset_cache.add(
			std::make_unique<Texture_Asset>( src_texture_tag, Rect( x, y, w, h ) ), std::format( "{}_21", tag.data() ), ""
		);

	// bottom row

	x = texture->rc.x;
	y = texture->rc.y + texture->rc.h - y_slices.b;
	w = x_slices.l;
	h = y_slices.b;

	asset_ptr->patches[ (int32_t)e_slice_def_patch::bottom_left ] =
		g_engine->asset_cache.add(
			std::make_unique<Texture_Asset>( src_texture_tag, Rect( x, y, w, h ) ), std::format( "{}_02", tag.data() ), ""
		);

	x = texture->rc.x + x_slices.l;
	w = texture->rc.w - x_slices.l - x_slices.r;
	asset_ptr->patches[ (int32_t)e_slice_def_patch::bottom_middle ] =
		g_engine->asset_cache.add(
			std::make_unique<Texture_Asset>( src_texture_tag, Rect( x, y, w, h ) ), std::format( "{}_12", tag.data() ), ""
		);

	x = texture->rc.x + texture->rc.w - x_slices.r;
	w = x_slices.r;
	asset_ptr->patches[ (int32_t)e_slice_def_patch::bottom_right ] =
		g_engine->asset_cache.add(
			std::make_unique<Texture_Asset>( src_texture_tag, Rect( x, y, w, h ) ), std::format( "{}_22", tag.data() ), ""
		);

	asset_ptr->create();
}

void Asset_File_Definition::precache_sound( const Key_Values& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "filename" ) );

	auto filename = std::format( "{}{}", data_folder, key_values_for_asset_def.find_value( "filename" ) );

	auto asset_ptr = g_engine->asset_cache.add( std::make_unique<Sound_Asset>(), tag.data(), filename );
	asset_ptr->original_filename = filename;

	for( auto& [key, value] : key_values_for_asset_def.kv )
	{
		if( key == "looped" )
		{
			asset_ptr->looped = bool( value == "true" );
		}
	}

	asset_ptr->create();
}

void Asset_File_Definition::precache_anim_texture( const Key_Values& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "frame_tags" ) );
	assert( key_values_for_asset_def.does_key_exist( "frames_per_sec" ) );
	assert( key_values_for_asset_def.does_key_exist( "tween" ) );

	auto frames_per_sec = Text_Parser::int_from_str( key_values_for_asset_def.find_value( "frames_per_sec" ) );
	auto tween_type = (e_tween_type)( Text_Parser::int_from_str( key_values_for_asset_def.find_value( "tween" ) ) );

	std::string_view frames = key_values_for_asset_def.find_value( "frame_tags" );

	std::vector<Texture_Asset*> anim_frames;
	Tokenizer tok( frames, "," );
	while( !tok.is_eos() )
	{
		anim_frames.push_back( g_engine->find_asset<Texture_Asset>( *tok.get_next_token() ) );
	}

	auto asset_ptr = g_engine->asset_cache.add(
		std::make_unique<Texture_Asset>( anim_frames, tween_type, frames_per_sec ),
		tag.data(),
		""
	);

	asset_ptr->create();
}

void Asset_File_Definition::precache_texture( const Key_Values& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "rect" ) );
	assert( key_values_for_asset_def.does_key_exist( "texture_tag" ) );

	Rect rc = Text_Parser::rect_from_str( key_values_for_asset_def.find_value( "rect" ) );
	auto asset_ptr = g_engine->asset_cache.add( std::make_unique<Texture_Asset>( key_values_for_asset_def.find_value( "texture_tag" ), rc ),
		tag.data(), "" );

	asset_ptr->create();
}

void Asset_File_Definition::precache_emitter_params( const Key_Values& key_values_for_asset_def, std::string_view tag )
{
	// ------------------------------------------------------------------------

	auto asset_ptr = g_engine->asset_cache.add( std::make_unique<Emitter_Parameters_Asset>(), tag.data(), "" );

	// ------------------------------------------------------------------------

	for( const auto& [key, value] : key_values_for_asset_def.kv )
	{
		if( key == "tag" or key == "type" )
		{
			// these are valid, we just don't need them here
		}
		else if( key == "b_needs_warm_up" )
		{
			asset_ptr->needs_warm_up = bool( value == "true" );
		}
		else if( key == "b_uses_gravity" )
		{
			asset_ptr->uses_gravity = bool( value == "true" );
		}
		else if( key == "b_one_shot" )
		{
			asset_ptr->is_one_shot = bool( value == "true" );
		}
		else if( key == "texture_tag" )
		{
			asset_ptr->texture = g_engine->find_asset<Texture_Asset>( value );
		}
		else if( key == "spawner_type" )
		{
			asset_ptr->particle_spawner.parse_from_config_string( value );
		}
		else if( key == "a_dir" )
		{
			asset_ptr->a_dir = Text_Parser::float_from_str( value );
		}
		else if( key == "r_dir_var" )
		{
			asset_ptr->r_dir_var = Text_Parser::range_from_str( value );
		}
		else if( key == "r_scale_spawn" )
		{
			asset_ptr->r_scale_spawn = Text_Parser::range_from_str( value );
		}
		else if( key == "t_scale" )
		{
			asset_ptr->t_scale = *Text_Parser::timeline_from_str( e_timeline_type::float_type, value );
		}
		else if( key == "s_spawn_rate" )
		{
			asset_ptr->s_spawn_rate = Text_Parser::float_from_str( value );
		}
		else if( key == "s_max_alive" )
		{
			asset_ptr->s_max_alive = Text_Parser::float_from_str( value );
		}
		else if( key == "r_lifespan" )
		{
			asset_ptr->r_lifespan = Text_Parser::range_from_str( value );
		}
		else if( key == "r_velocity_spawn" )
		{
			asset_ptr->r_velocity_spawn = Text_Parser::range_from_str( value );
		}
		else if( key == "t_color" )
		{
			asset_ptr->t_color = *Text_Parser::timeline_from_str( e_timeline_type::color_type, value );
		}
		else if( key == "r_spin_spawn" )
		{
			asset_ptr->r_spin_spawn = Text_Parser::range_from_str( value );
		}
		else if( key == "r_spin_per_sec" )
		{
			asset_ptr->r_spin_per_sec = Text_Parser::range_from_str( value );
		}
		else if( key == "t_alpha" )
		{
			asset_ptr->t_alpha = *Text_Parser::timeline_from_str( e_timeline_type::float_type, value );
		}
		else
		{
			log_warning( "Unknown key read from config block : [emitter tag:{} / key:\"{}\"]", tag, key );
		}
	}

	asset_ptr->create();
}

void Asset_File_Definition::precache_font( const Key_Values& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "font_def_tag" ) );

	auto asset_ptr = g_engine->asset_cache.add( std::make_unique<Font_Asset>(), tag.data(), "" );
	asset_ptr->font_def = g_engine->find_asset<Font_Def_Asset>( key_values_for_asset_def.find_value( "font_def_tag" ) );
	asset_ptr->create();
}

void Asset_File_Definition::precache_mesh( const Key_Values& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "filename" ) );

	auto filename = std::format( "{}{}", data_folder, key_values_for_asset_def.find_value( "filename" ) );

	auto asset_ptr = g_engine->asset_cache.add( std::make_unique<Mesh_Asset>(), tag.data(), filename );

	asset_ptr->original_filename = filename;

	asset_ptr->create();
}

void Asset_File_Definition::precache_cursor( const Key_Values& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "texture_tag" ) );

	auto asset_ptr = g_engine->asset_cache.add( std::make_unique<Cursor_Asset>(), tag.data(), "" );

	asset_ptr->texture = g_engine->find_asset<Texture_Asset>( key_values_for_asset_def.find_value( "texture_tag" ) );
	asset_ptr->hotspot_offset = Text_Parser::vec2_from_str( key_values_for_asset_def.find_value( "hotspot" ) );

	asset_ptr->create();
}

void Asset_File_Definition::precache_tile_set( const Key_Values& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "texture_tag" ) );
	assert( key_values_for_asset_def.does_key_exist( "filename" ) );

	auto filename = std::format( "{}{}", data_folder, key_values_for_asset_def.find_value( "filename" ) );
	auto asset_ptr = g_engine->asset_cache.add( std::make_unique<Tile_Set_Asset>(), tag.data(), filename );
	asset_ptr->original_filename = filename;

	asset_ptr->texture = g_engine->find_asset<Texture_Asset>( key_values_for_asset_def.find_value( "texture_tag" ) );

	asset_ptr->create();
}

void Asset_File_Definition::precache_tile_map( const Key_Values& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "filename" ) );

	auto filename = std::format( "{}{}", data_folder, key_values_for_asset_def.find_value( "filename" ) );

	auto asset_ptr = g_engine->asset_cache.add( std::make_unique<Tile_Map_Asset>(), tag.data(), filename );

	asset_ptr->original_filename = filename;

	asset_ptr->create();
}

// ----------------------------------------------------------------------------

bool Asset_File_Definition::create_internals()
{
	asset_definitions.clear();

	// read the contents of the asset definition file and break it up
	// into individual asset definitions

	auto file = File_System::load_text_file( original_filename );

	Key_Values current_asset_definition;

	// loop through every line of the asset_def fil and
	for( const auto& line : file->lines )
	{
		// a "{" marks the beginning of a new asset definition
		if( line[ 0 ] == '{' )
		{
			current_asset_definition = {};
		}
		// a "}" marks the end of the current asset definition
		else if( line[ 0 ] == '}' )
		{
			asset_definitions.push_back( current_asset_definition );
			current_asset_definition = {};
		}
		// parse each line into a key/value pair for the current asset definition
		else
		{
			Tokenizer tok_kv( line, "\"" );

			auto key = tok_kv.get_next_token();
			tok_kv.get_next_token();
			auto value = tok_kv.get_next_token();

			if( key.has_value() and value.has_value() )
			{
				current_asset_definition.kv.insert_or_assign( std::string( *key ), std::string( *value ) );
			}
		}
	}

	return true;
}

}
