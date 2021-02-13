
#include "master_pch.h"
#include "master_header.h"

void w_asset_definition_file::precache_asset_resources( size_t pass_num )
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
				else if( type == "gradient" )		precache_gradient( key_values_for_asset_def, tag );
				else if( type == "font_def" )		precache_font_def( key_values_for_asset_def, tag );
				else if( type == "slice_def" )		precache_slice_def( key_values_for_asset_def, tag );
				else if( type == "sound" )			precache_sound( key_values_for_asset_def, tag );
				else if( type == "anim_texture" )	precache_anim_texture( key_values_for_asset_def, tag );
				else if( type == "texture" )		precache_texture( key_values_for_asset_def, tag );
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
		}
	}
}

void w_asset_definition_file::precache_preproc( const w_keyvalues& key_values_for_asset_def )
{
	for( auto& [key, value] : key_values_for_asset_def.kv )
	{
		if( key != "tag" && key != "type" )
		{
			engine->_symbol_to_value.insert_or_assign( key, value );
		}
	}
}

void w_asset_definition_file::precache_palette( const w_keyvalues& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "colors" ) );

	auto asset_ptr = engine->asset_cache->add( std::make_unique<a_palette>(), tag.data(), "" );
	asset_ptr->colors = w_parser::color_list_from_str( key_values_for_asset_def.find_value( "colors" ) );
	asset_ptr->clean_up_internals();
	asset_ptr->create_internals();
}

void w_asset_definition_file::precache_src_texture( const w_keyvalues& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "filename" ) );

	auto filename = fmt::format( "{}{}", data_folder, key_values_for_asset_def.find_value( "filename" ) );

	auto asset_ptr = engine->asset_cache->add( std::make_unique<a_src_texture>(), tag.data(), filename );
	asset_ptr->original_filename = filename;
	asset_ptr->tiling = engine->find_int_from_symbol( key_values_for_asset_def.find_value_opt( "tiling", "tiling.clamp" ) );

	asset_ptr->clean_up_internals();
	asset_ptr->create_internals();

	// ----------------------------------------------------------------------------
	// every src_texture automatically creates an a_texture with the same name

	engine->asset_cache->add( std::make_unique<a_texture>( tag ), tag.data(), "" );

	// the texture_tags" k/v is a convenient way to specify a set
	// of textures belonging to a src_texture. it's an easy way to break
	// down atlas textures or sprite sheets into individual textures.

	if( key_values_for_asset_def.does_key_exist( "texture_tags" ) )
	{
		std::string_view texture_list = key_values_for_asset_def.find_value( "texture_tags" );

		int comma_count = static_cast<int>( std::count( texture_list.begin(), texture_list.end(), ',' ) );

		// each texture_tag is expected to have 5 args : name,x,y,w,h
		if( ( comma_count % 5 ) != 4 )
		{
			log_error( "'{}' has bad formatting - too many or too few commas", texture_list );
		}

		w_tokenizer tok( texture_list, ',' );

		while( !tok.is_eos() )
		{
			auto texture_tag = std::string( *tok.get_next_token() );

			float x = w_parser::float_from_str( *tok.get_next_token() );
			float y = w_parser::float_from_str( *tok.get_next_token() );
			float w = w_parser::float_from_str( *tok.get_next_token() );
			float h = w_parser::float_from_str( *tok.get_next_token() );

			w_rect rc( x, y, w, h );

			engine->asset_cache->add( std::make_unique<a_texture>( tag, rc ), texture_tag, "" );
		}
	}
}

void w_asset_definition_file::precache_gradient( const w_keyvalues& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "alignment" ) );
	assert( key_values_for_asset_def.does_key_exist( "colors" ) );

	auto asset_ptr = engine->asset_cache->add( std::make_unique<a_gradient>(), tag.data(), "" );

	asset_ptr->alignment = static_cast<e_align>( engine->find_int_from_symbol( key_values_for_asset_def.find_value( "alignment" ) ) );

	asset_ptr->colors.clear();
	std::vector<w_color> color_list = w_parser::color_list_from_str( key_values_for_asset_def.find_value( "colors" ) );

	// must reverse the order or else the gradient texture
	// ends up backwards on the screen
	std::reverse( color_list.begin(), color_list.end() );

	for( const auto& iter : color_list )
	{
		asset_ptr->colors.push_back( iter.r );
		asset_ptr->colors.push_back( iter.g );
		asset_ptr->colors.push_back( iter.b );
		asset_ptr->colors.push_back( iter.a );
	}

	// every gradient automatically creates an a_texture with the same name

	engine->asset_cache->add( std::make_unique<a_texture>( tag.data() ), tag.data(), "" );

	asset_ptr->clean_up_internals();
	asset_ptr->create_internals();
}

void w_asset_definition_file::precache_font_def( const w_keyvalues& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "filename" ) );
	assert( key_values_for_asset_def.does_key_exist( "src_texture_tag" ) );

	auto filename = fmt::format( "{}{}", data_folder, key_values_for_asset_def.find_value( "filename" ) );

	auto asset_ptr = engine->asset_cache->add( std::make_unique<a_font_def>(), tag.data(), filename );

	asset_ptr->original_filename = filename;
	asset_ptr->src_texture = a_src_texture::find( key_values_for_asset_def.find_value( "src_texture_tag" ) );

	asset_ptr->clean_up_internals();
	asset_ptr->create_internals();
}

void w_asset_definition_file::precache_slice_def( const w_keyvalues& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "texture_tag" ) );
	assert( key_values_for_asset_def.does_key_exist( "x_slices" ) );
	assert( key_values_for_asset_def.does_key_exist( "y_slices" ) );

	auto asset_ptr = engine->asset_cache->add( std::make_unique<a_9slice_def>(), tag.data(), "" );

	a_texture* texture = a_texture::find( key_values_for_asset_def.find_value( "texture_tag" ) );
	w_vec2 x_slices = w_parser::vec2_from_str( key_values_for_asset_def.find_value( "x_slices" ) );
	w_vec2 y_slices = w_parser::vec2_from_str( key_values_for_asset_def.find_value( "y_slices" ) );

	float x, y, w, h;

	// top row

	x = texture->rc.x;
	y = texture->rc.y;
	w = x_slices.l;
	h = y_slices.t;

	asset_ptr->patches[ slicedef_patch::P_00 ] =
		engine->asset_cache->add(
			std::make_unique<a_texture>( texture->src_texture->tag.data(), w_rect( x, y, w, h ) ), fmt::format( "{}_00", tag.data() ), ""
		);

	x = texture->rc.x + x_slices.l;
	w = texture->rc.w - x_slices.l - x_slices.r;
	asset_ptr->patches[ slicedef_patch::P_10 ] =
		engine->asset_cache->add(
			std::make_unique<a_texture>( texture->src_texture->tag.data(), w_rect( x, y, w, h ) ), fmt::format( "{}_10", tag.data() ), ""
		);

	x = texture->rc.x + texture->rc.w - x_slices.r;
	w = x_slices.r;
	asset_ptr->patches[ slicedef_patch::P_20 ] =
		engine->asset_cache->add(
			std::make_unique<a_texture>( texture->src_texture->tag.data(), w_rect( x, y, w, h ) ), fmt::format( "{}_20", tag.data() ), ""
		);

	// middle row

	x = texture->rc.x;
	y = texture->rc.y + y_slices.t;
	w = x_slices.l;
	h = texture->rc.h - y_slices.t - y_slices.b;

	asset_ptr->patches[ slicedef_patch::P_01 ] =
		engine->asset_cache->add(
			std::make_unique<a_texture>( texture->src_texture->tag.data(), w_rect( x, y, w, h ) ), fmt::format( "{}_01", tag.data() ), ""
		);

	x = texture->rc.x + x_slices.l;
	w = texture->rc.w - x_slices.l - x_slices.r;
	asset_ptr->patches[ slicedef_patch::P_11 ] =
		engine->asset_cache->add(
			std::make_unique<a_texture>( texture->src_texture->tag.data(), w_rect( x, y, w, h ) ), fmt::format( "{}_11", tag.data() ), ""
		);

	x = texture->rc.x + texture->rc.w - x_slices.r;
	w = x_slices.r;
	asset_ptr->patches[ slicedef_patch::P_21 ] =
		engine->asset_cache->add(
			std::make_unique<a_texture>( texture->src_texture->tag.data(), w_rect( x, y, w, h ) ), fmt::format( "{}_21", tag.data() ), ""
		);

	// bottom row

	x = texture->rc.x;
	y = texture->rc.y + texture->rc.h - y_slices.b;
	w = x_slices.l;
	h = y_slices.b;

	asset_ptr->patches[ slicedef_patch::P_02 ] =
		engine->asset_cache->add(
			std::make_unique<a_texture>( texture->src_texture->tag.data(), w_rect( x, y, w, h ) ), fmt::format( "{}_02", tag.data() ), ""
		);

	x = texture->rc.x + x_slices.l;
	w = texture->rc.w - x_slices.l - x_slices.r;
	asset_ptr->patches[ slicedef_patch::P_12 ] =
		engine->asset_cache->add(
			std::make_unique<a_texture>( texture->src_texture->tag.data(), w_rect( x, y, w, h ) ), fmt::format( "{}_12", tag.data() ), ""
		);

	x = texture->rc.x + texture->rc.w - x_slices.r;
	w = x_slices.r;
	asset_ptr->patches[ slicedef_patch::P_22 ] =
		engine->asset_cache->add(
			std::make_unique<a_texture>( texture->src_texture->tag.data(), w_rect( x, y, w, h ) ), fmt::format( "{}_22", tag.data() ), ""
		);

	asset_ptr->clean_up_internals();
	asset_ptr->create_internals();
}

void w_asset_definition_file::precache_sound( const w_keyvalues& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "filename" ) );

	auto filename = fmt::format( "{}{}", data_folder, key_values_for_asset_def.find_value( "filename" ) );

	auto asset_ptr = engine->asset_cache->add( std::make_unique<a_sound>(), tag.data(), filename );
	asset_ptr->original_filename = filename;

	for( auto& [key, value] : key_values_for_asset_def.kv )
	{
		if( key == "looped" )
		{
			asset_ptr->looped = bool( value == "true" );
		}
	}

	asset_ptr->clean_up_internals();
	asset_ptr->create_internals();
}

void w_asset_definition_file::precache_anim_texture( const w_keyvalues& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "frame_texture_tags" ) );
	assert( key_values_for_asset_def.does_key_exist( "frames_per_sec" ) );
	assert( key_values_for_asset_def.does_key_exist( "tween" ) );

	int frames_per_sec = w_parser::int_from_str( key_values_for_asset_def.find_value( "frames_per_sec" ) );
	auto tween_type = static_cast<e_tween_type>( w_parser::int_from_str( key_values_for_asset_def.find_value( "tween" ) ) );

	auto asset_ptr = engine->asset_cache->add( std::make_unique<a_anim_texture>( tween_type, frames_per_sec ), tag.data(), "" );

	const std::string_view frames = key_values_for_asset_def.find_value( "frame_texture_tags" );

	w_tokenizer tok( frames, ',' );
	asset_ptr->frames.reserve( tok.tokens.size() );
	while( !tok.is_eos() )
	{
		auto texture = a_texture::find( *tok.get_next_token() );
		asset_ptr->add_frame( texture );
	}

	asset_ptr->clean_up_internals();
	asset_ptr->create_internals();
}

void w_asset_definition_file::precache_texture( const w_keyvalues& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "rect" ) );
	assert( key_values_for_asset_def.does_key_exist( "texture_tag" ) );

	w_rect rc = w_parser::rect_from_str( key_values_for_asset_def.find_value( "rect" ) );
	auto asset_ptr = engine->asset_cache->add( std::make_unique<a_texture>( key_values_for_asset_def.find_value( "texture_tag" ), rc ),
		tag.data(), "" );

	asset_ptr->clean_up_internals();
	asset_ptr->create_internals();
}

void w_asset_definition_file::precache_emitter_params( const w_keyvalues& key_values_for_asset_def, std::string_view tag )
{
	// ------------------------------------------------------------------------

	auto asset_ptr = engine->asset_cache->add( std::make_unique<a_emitter_params>(), tag.data(), "" );

	// ------------------------------------------------------------------------

	for( const auto& [key, value] : key_values_for_asset_def.kv )
	{
		if( key == "tag" || key == "type" )
		{
			// these are valid, we just don't need them here
		}
		else if( key == "b_needs_warm_up" )
		{
			asset_ptr->needs_warm_up = bool( value == "true" );
		}
		else if( key == "b_one_shot" )
		{
			asset_ptr->is_one_shot = bool( value == "true" );
		}
		else if( key == "texture_tag" )
		{
			asset_ptr->texture = a_texture::find( value );
		}
		else if( key == "spawner_type" )
		{
			asset_ptr->particle_spawner.parse_from_config_string( value );
		}
		else if( key == "a_dir" )
		{
			asset_ptr->a_dir = w_parser::float_from_str( value );
		}
		else if( key == "r_dir_var" )
		{
			asset_ptr->r_dir_var = w_parser::range_from_str( value );
		}
		else if( key == "r_scale_spawn" )
		{
			asset_ptr->r_scale_spawn = w_parser::range_from_str( value );
		}
		else if( key == "t_scale" )
		{
			asset_ptr->t_scale = *w_parser::timeline_from_str( timeline_type::float_type, value );
		}
		else if( key == "s_max_spawn_per_sec" )
		{
			asset_ptr->s_max_spawn_per_sec = w_parser::float_from_str( value );
		}
		else if( key == "r_lifespan" )
		{
			asset_ptr->r_lifespan = w_parser::range_from_str( value );
		}
		else if( key == "r_velocity_spawn" )
		{
			asset_ptr->r_velocity_spawn = w_parser::range_from_str( value );
		}
		else if( key == "t_color" )
		{
			asset_ptr->t_color = *w_parser::timeline_from_str( timeline_type::color_type, value );
		}
		else if( key == "r_spin_spawn" )
		{
			asset_ptr->r_spin_spawn = w_parser::range_from_str( value );
		}
		else if( key == "r_spin_per_sec" )
		{
			asset_ptr->r_spin_per_sec = w_parser::range_from_str( value );
		}
		else if( key == "t_alpha" )
		{
			asset_ptr->t_alpha = *w_parser::timeline_from_str( timeline_type::float_type, value );
		}
		else
		{
			log_warning( "Unknown key read from config block : [emitter tag:{} / key:\"{}\"]", tag, key );
		}
	}

	asset_ptr->clean_up_internals();
	asset_ptr->create_internals();
}

void w_asset_definition_file::precache_font( const w_keyvalues& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "font_def_tag" ) );

	auto asset_ptr = engine->asset_cache->add( std::make_unique<a_font>(), tag.data(), "" );
	asset_ptr->font_def = a_font_def::find( key_values_for_asset_def.find_value( "font_def_tag" ) );
	asset_ptr->clean_up_internals();
	asset_ptr->create_internals();
}

void w_asset_definition_file::precache_mesh( const w_keyvalues& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "filename" ) );
	assert( key_values_for_asset_def.does_key_exist( "texture_tag" ) );

	auto filename = fmt::format( "{}{}", data_folder, key_values_for_asset_def.find_value( "filename" ) );

	auto asset_ptr = engine->asset_cache->add( std::make_unique<a_mesh>(), tag.data(), filename );

	asset_ptr->tex = a_texture::find( key_values_for_asset_def.find_value( "texture_tag" ) );
	asset_ptr->original_filename = filename;

	asset_ptr->clean_up_internals();
	asset_ptr->create_internals();
}

void w_asset_definition_file::precache_cursor( const w_keyvalues& key_values_for_asset_def, std::string_view tag )
{
	assert( key_values_for_asset_def.does_key_exist( "texture_tag" ) );

	auto asset_ptr = engine->asset_cache->add( std::make_unique<a_cursor>(), tag.data(), "" );

	asset_ptr->texture = a_texture::find( key_values_for_asset_def.find_value( "texture_tag" ) );
	asset_ptr->hotspot_offset = w_parser::vec2_from_str( key_values_for_asset_def.find_value( "hotspot" ) );

	asset_ptr->clean_up_internals();
	asset_ptr->create_internals();
}

bool w_asset_definition_file::create_internals()
{
	asset_definitions.clear();

	// read the contents of the asset definition file and break it up
	// into individual asset definitions

	auto file = engine->fs->load_text_file( original_filename );

	w_keyvalues current_asset_definition;

	// loop through every line of the asset_def fil and
	for( const auto& line : *( file->lines.get() ) )
	{
		// a "{" marks the beginning of a new asset definition
		if( line[ 0 ] == '{' )
		{
			current_asset_definition = {};
		}
		// a "}" marks the end of the current asset definition
		else if( line[0] == '}' )
		{
			asset_definitions.emplace_back( std::move( current_asset_definition ) );
			current_asset_definition = {};
		}
		// parse each line into a key/value pair for the current asset definition
		else
		{
			w_tokenizer tok_kv( line, '\"' );

			auto key = tok_kv.get_next_token();
			tok_kv.get_next_token();
			auto value = tok_kv.get_next_token();

			if( key.has_value() && value.has_value() )
			{
				current_asset_definition.insert_or_assign( *key, *value );
			}
		}
	}

	return true;
}
