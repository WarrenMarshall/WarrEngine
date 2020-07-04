#include "master_pch.h"
#include "master_header.h"

w_asset_definition_file::~w_asset_definition_file()
{
	clean_up_internals();
}

void w_asset_definition_file::precache_asset_resources( int pass_num, bool is_hot_reloading )
{
	std::string type, name, filename;

	for( const auto& iter_ad : asset_definitions )
	{
		assert_key_exists( "??", *(iter_ad.get()), "type" );
		assert_key_exists( "??", *( iter_ad.get() ), "name" );

		type = iter_ad->key_values.at( "type" );
		name = iter_ad->key_values.at( "name" );
		filename = "";

		switch( pass_num )
		{
			case 0:
			{
				if( type == "preproc" )
				{
					for( auto& iter : iter_ad->key_values )
					{
						std::string key = iter.first;
						std::string value = iter.second;

						if( key != "name" && key != "type" )
							engine->_symbol_to_value[ key ] = value;
					}
				}
			}
			break;

			case 1:
			{
				if( type == "texture" )
				{
					assert_key_exists( type, *( iter_ad.get() ), "filename" );

					filename = iter_ad->key_values.at( "filename" );

					// ------------------------------------------------------------------------

					auto asset_ptr = engine->get_asset<a_texture>( name, b_silent(true) );

					if( !asset_ptr )
						asset_ptr = static_cast<a_texture*>(
							engine->asset_cache->add( std::make_unique<a_texture>(), name, filename )
							);

					// ------------------------------------------------------------------------

					asset_ptr->original_filename = filename;

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals( is_hot_reloading );

					// Every texture that gets loaded gets a full size
					// a_subtexture created for it automatically.

					auto subtex = static_cast<a_subtexture*>(
						engine->asset_cache->add( std::make_unique<a_subtexture>( asset_ptr->name ),
												  "auto_subt_" + asset_ptr->name, "" )
						);
					asset_ptr->subtex = subtex;
				}
				else if( type == "gradient" )
				{
					assert_key_exists( type, *( iter_ad.get() ), "alignment" );
					assert_key_exists( type, *( iter_ad.get() ), "colors" );

					// ------------------------------------------------------------------------

					auto asset_ptr = engine->get_asset<a_gradient>( name, b_silent( true ) );

					if( !asset_ptr )
					{
						asset_ptr = static_cast<a_gradient*>(
							engine->asset_cache->add( std::make_unique<a_gradient>(), name, "" )
							);
					}

					// ------------------------------------------------------------------------

					asset_ptr->alignment = static_cast<e_align>( engine->find_int_from_symbol( iter_ad->key_values.at( "alignment" ) ) );

					asset_ptr->colors.clear();

					std::string color_list = iter_ad->key_values.at( "colors" );

					w_tokenizer tok( color_list, '/', "n/a" );
					std::string val;

					std::vector<std::string> color_values;
					while( true )
					{
						val = tok.get_next_token();
						if( val == "n/a" )
							break;

						color_values.emplace_back( val );
					}

					for( const auto& iter : color_values )
					{
						val = iter;

						if( engine->is_symbol_in_map( val ) )
							val = engine->find_val_from_symbol( val );

						asset_ptr->colors.emplace_back( w_color( val ) );
					}

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals( is_hot_reloading );
				}
				else if( type == "font_def" )
				{
					assert_key_exists( type, *( iter_ad.get() ), "filename" );
					assert_key_exists( type, *( iter_ad.get() ), "texture" );

					filename = iter_ad->key_values.at( "filename" );

					// ------------------------------------------------------------------------

					auto asset_ptr = engine->get_asset<a_font_def>( name, b_silent( true ) );

					if( !asset_ptr )
						asset_ptr = static_cast<a_font_def*>(
							engine->asset_cache->add( std::make_unique<a_font_def>(), name, filename )
							);

					// ------------------------------------------------------------------------

					asset_ptr->original_filename = filename;
					asset_ptr->texture_name = iter_ad->key_values.at( "texture" );

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals( is_hot_reloading );
				}
				else if( type == "9slice_def" )
				{
					assert_key_exists( type, *( iter_ad.get() ), "texture" );

					// ------------------------------------------------------------------------

					auto asset_ptr = engine->get_asset<a_9slice_def>( name, b_silent( true ) );

					if( !asset_ptr )
						asset_ptr = static_cast<a_9slice_def*>(
							engine->asset_cache->add( std::make_unique<a_9slice_def>(), name, filename )
							);

					// ------------------------------------------------------------------------

					std::string tex_name = iter_ad->key_values.at( "texture" );

					for( const auto& iter : iter_ad->key_values )
					{
						std::string key = iter.first;
						std::string value = iter.second;

						int subtexture_idx = 0;

						if( key.substr( 0, 2 ) == "p_" )
						{
							subtexture_idx = w_parser::parse_int_value( key );
							w_rect rc = w_parser::parse_rect_value( value );

							std::string subtex_name = name + "_" + key;

							asset_ptr->patches[ subtexture_idx ] = static_cast<a_subtexture*>(
								engine->asset_cache->add(
									std::make_unique<a_subtexture>( iter_ad->key_values.at( "texture" ), rc ), subtex_name, ""
								)
							);
						}
					}

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals( is_hot_reloading );
				}
				else if( type == "sound" )
				{
					assert_key_exists( type, *( iter_ad.get() ), "filename" );

					filename = iter_ad->key_values.at( "filename" );

					// ------------------------------------------------------------------------

					auto asset_ptr = engine->get_asset<a_sound>( name, b_silent( true ) );

					if( !asset_ptr )
						asset_ptr = static_cast<a_sound*>(
							engine->asset_cache->add( std::make_unique<a_sound>(), name, filename )
							);

					// ------------------------------------------------------------------------

					asset_ptr->original_filename = filename;

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals( is_hot_reloading );
				}
				else if( type == "music" )
				{
					assert_key_exists( type, *( iter_ad.get() ), "filename" );

					filename = iter_ad->key_values.at( "filename" );

					// ------------------------------------------------------------------------

					auto asset_ptr = engine->get_asset<a_music>( name, b_silent( true ) );

					if( !asset_ptr )
						asset_ptr = static_cast<a_music*>(
							engine->asset_cache->add( std::make_unique<a_music>(), name, filename )
							);

					// ------------------------------------------------------------------------

					asset_ptr->original_filename = filename;

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals( is_hot_reloading );
				}
			}
			break;

			case 2:
			{
				if( type == "emitter_params" )
				{
					// ------------------------------------------------------------------------

					auto asset_ptr = engine->get_asset<a_emitter_params>( name, b_silent( true ) );

					if( !asset_ptr )
						asset_ptr = static_cast<a_emitter_params*>(
							engine->asset_cache->add( std::make_unique<a_emitter_params>(), name, "" )
							);

					// ------------------------------------------------------------------------

					for( const auto& iter : iter_ad->key_values )
					{
						std::string key = iter.first;
						std::string value = iter.second;

						if( key == "name" || key == "type" )
						{
							// these are valid, we just don't need them here
						}
						else if( key == "b_needs_warm_up" )
							asset_ptr->b_needs_warm_up = bool( value == "true" );
						else if( key == "b_one_shot" )
							asset_ptr->b_one_shot = bool( value == "true" );
						else if( key == "texture_name" )
							asset_ptr->tex = engine->get_asset<a_texture>( value );
						else if( key == "spawner_type" )
						{
							w_tokenizer tok( value, ',' );
							std::string type = tok.get_next_token();

							if( type == "point" )
							{
								// the default spawner type, don't need to do anything for this
							}
							else if( type == "box" )
								asset_ptr->particle_spawner = std::make_unique<w_particle_spawner_box>();
							else if( type == "circle" )
								asset_ptr->particle_spawner = std::make_unique<w_particle_spawner_circle>();
							else
								log_error( "%s : unknown emitter spawn type : [%s]", __FUNCTION__, type.c_str() );

							asset_ptr->particle_spawner->parse_from_config_string( value );
						}
						else if( key == "a_dir" )
							asset_ptr->a_dir = w_parser::parse_float_value( value );
						else if( key == "r_dir_var" )
							asset_ptr->r_dir_var = w_parser::parse_range_value( value );
						else if( key == "r_scale_spawn" )
							asset_ptr->r_scale_spawn = w_parser::parse_range_value( value );
						else if( key == "t_scale" )
							asset_ptr->t_scale = w_parser::parse_timeline_value( e_timeline_type::float_type, value );
						else if( key == "s_max_spawn_per_sec" )
							asset_ptr->s_max_spawn_per_sec = w_parser::parse_float_value( value );
						else if( key == "r_lifespan" )
							asset_ptr->r_lifespan = w_parser::parse_range_value( value );
						else if( key == "r_velocity_spawn" )
							asset_ptr->r_velocity_spawn = w_parser::parse_range_value( value );
						else if( key == "t_color" )
							asset_ptr->t_color = w_parser::parse_timeline_value( e_timeline_type::color_type, value );
						else if( key == "r_spin_spawn" )
							asset_ptr->r_spin_spawn = w_parser::parse_range_value( value );
						else if( key == "r_spin_per_sec" )
							asset_ptr->r_spin_per_sec = w_parser::parse_range_value( value );
						else if( key == "t_alpha" )
							asset_ptr->t_alpha = w_parser::parse_timeline_value( e_timeline_type::float_type, value );
						else
							log_msg( "%s : unknown key read from config block : [%s -> \"%s\"]", __FUNCTION__, name.c_str(), key.c_str() );
					}

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals( is_hot_reloading );
				}
				else if( type == "font" )
				{
					assert_key_exists( type, *( iter_ad.get() ), "font_def" );

					// ------------------------------------------------------------------------

					auto asset_ptr = engine->get_asset<a_font>( name, b_silent( true ) );

					if( !asset_ptr )
						asset_ptr = static_cast<a_font*>(
							engine->asset_cache->add( std::make_unique<a_font>(), name, "" )
							);

					// ------------------------------------------------------------------------

					asset_ptr->font_def = engine->get_asset<a_font_def>( iter_ad->key_values.at( "font_def" ) );

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals( is_hot_reloading );
				}
				else if( type == "cursor" )
				{
					assert_key_exists( type, *( iter_ad.get() ), "subtexture" );
					assert_key_exists( type, *( iter_ad.get() ), "hotspot" );

					// ------------------------------------------------------------------------

					auto asset_ptr = engine->get_asset<a_cursor>( name, b_silent( true ) );

					if( !asset_ptr )
						asset_ptr = static_cast<a_cursor*>(
							engine->asset_cache->add( std::make_unique<a_cursor>(), name, "" )
							);

					// ------------------------------------------------------------------------

					asset_ptr->img = engine->get_asset<a_subtexture>( iter_ad->key_values.at( "subtexture" ) );
					asset_ptr->hotspot_offset = w_parser::parse_vec2_value( iter_ad->key_values.at( "hotspot" ) );

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals( is_hot_reloading );
				}
				else if( type == "anim_texture" )
				{
					assert_key_exists( type, *( iter_ad.get() ), "frames" );
					assert_key_exists( type, *( iter_ad.get() ), "tween" );
					assert_key_exists( type, *( iter_ad.get() ), "frames_per_sec" );

					auto asset_ptr = engine->get_asset<a_anim_texture>( name, b_silent( true ) );

					int frames_per_sec = w_parser::parse_int_value( iter_ad->key_values.at( "frames_per_sec" ) );
					e_tween_type tween_type = static_cast<e_tween_type>( w_parser::parse_int_value( iter_ad->key_values.at( "tween" ) ) );

					if( !asset_ptr )
					{
						asset_ptr = static_cast<a_anim_texture*>(
							engine->asset_cache->add(
								std::make_unique<a_anim_texture>( tween_type, frames_per_sec ),
								name, "" )
							);
					}

					// ------------------------------------------------------------------------

					const std::string frames = iter_ad->key_values.at( "frames" );

					w_tokenizer tok( frames, ',' );
					while( !tok.is_eos() )
					{
						auto tex = engine->get_asset<a_subtexture>( tok.get_next_token() );
						asset_ptr->frames.emplace_back( tex );
					}

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals( is_hot_reloading );
				}
				else if( type == "subtexture" )
				{
					assert_key_exists( type, *( iter_ad.get() ), "texture" );
					assert_key_exists( type, *( iter_ad.get() ), "rect" );

					// ------------------------------------------------------------------------

					auto asset_ptr = engine->get_asset<a_subtexture>( name, b_silent( true ) );

					if( !asset_ptr )
					{
						w_rect rc = w_parser::parse_rect_value( iter_ad->key_values.at( "rect" ) );
						asset_ptr = static_cast<a_subtexture*>(
							engine->asset_cache->add( std::make_unique<a_subtexture>( iter_ad->key_values.at( "texture" ), rc ), name, "" )
							);
					}

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals( is_hot_reloading );
				}
			}
			break;
		}
	}
}

void w_asset_definition_file::assert_key_exists( const std::string& type, const w_asset_definition& asset_def, const std::string& key )
{
	#if !defined(FINALRELEASE)
		if( !asset_def.key_values.count( key ) )
			log_error( "%s : %s : asset definition missing '%s' key/value", __FUNCTION__, type.c_str(), key.c_str() );
	#endif
}

void w_asset_definition_file::clean_up_internals()
{
}

bool w_asset_definition_file::create_internals( bool is_hot_reloading )
{
	asset_definitions.clear();

	// read the contents of the asset definition file and break it up
	// into individual asset definitions

	auto file = engine->fs->load_file_into_memory( original_filename );
	was_loaded_from_zip_file = file->was_loaded_from_zip_file;

	w_mem_file* mf = file.get();
	std::string file_as_string = std::string( file->buffer->begin(), file->buffer->end() );

	w_tokenizer tok( file_as_string, '\n', "" );

	std::string line = tok.get_next_token();
	std::unique_ptr<w_asset_definition> current_asset_definition = nullptr;

	while( !tok.is_eos() )
	{
		// ignore blank lines and lines that are entirely commented
		if( line.size() > 0 && line[0] != '#' )
		{
			while( line.back() == '\\' )
			{
				line = line.substr( 0, line.length() - 1 );
				line += tok.get_next_token();
			}

			// strip comments off the back ends of lines
			size_t pos = line.find_first_of( "#" );

			if( pos != std::string::npos )
				line = w_stringutil::rtrim( line.substr( 0, pos ) );

			if( line[0] == '{' )
				current_asset_definition = std::make_unique<w_asset_definition>();
			else if( line[0] == '}' )
			{
				asset_definitions.emplace_back( std::move( current_asset_definition ) );
				current_asset_definition = nullptr;
			}
			else
			{
				w_tokenizer tok( line, '\"' );

				tok.get_next_token();
				std::string key = tok.get_next_token();
				tok.get_next_token();
				std::string value = tok.get_next_token();

				current_asset_definition->key_values.insert( std::make_pair( key, value ) );
			}
		}

		line = tok.get_next_token();
	}

	if( g_allow_hot_reload )
	{
		// save the last time modified for hot reloading
		last_modified = get_last_modified_from_disk();

		if( is_hot_reloading )
			for( int p = 0; p < w_engine::num_asset_def_passes; ++p )
				precache_asset_resources( p, is_hot_reloading );
	}

	return true;
}
