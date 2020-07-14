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
		type = iter_ad->find_value( "type" );
		name = iter_ad->find_value( "name" );
		filename = "";

		switch( pass_num )
		{
			case 0:
			{
				if( type == "preproc" )
				{
					for( auto& iter : *(iter_ad->data()) )
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
					filename = iter_ad->find_value( "filename");

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

					// if the "subtexture" key exists, create a subtexture for this texture
					// that represents it's entirety.

					if( iter_ad->does_key_exist( "subtexture" ) )
					{
						auto subtex = static_cast<a_subtexture*>(
							engine->asset_cache->add( std::make_unique<a_subtexture>( name ),
													  iter_ad->find_value( "subtexture" ), "" )
							);

						// #todo - is this subtex pointer inside a_texture still useful?
						asset_ptr->subtex = subtex;
					}

					// the "subtextures" k/v is a convenient way to specify a set
					// of subtextures belonging to a texture. an easy way to break
					// down atlas textures or sprite sheets into subtextures.

					if( iter_ad->does_key_exist( "subtextures" ) )
					{
						std::string subtex_list = iter_ad->find_value( "subtextures" );

						w_tokenizer tok( subtex_list, ',' );

						while( !tok.is_eos() )
						{
							std::string subtex_name = tok.get_next_token();

							float x = w_parser::float_from_str( tok.get_next_token() );
							float y = w_parser::float_from_str( tok.get_next_token() );
							float w = w_parser::float_from_str( tok.get_next_token() );
							float h = w_parser::float_from_str( tok.get_next_token() );

							w_rect rc( x, y, w, h );

							engine->asset_cache->add( std::make_unique<a_subtexture>( name, rc ), subtex_name, "" );
						}
					}
				}
				else if( type == "gradient" )
				{
					// ------------------------------------------------------------------------

					auto asset_ptr = engine->get_asset<a_gradient>( name, b_silent( true ) );

					if( !asset_ptr )
					{
						asset_ptr = static_cast<a_gradient*>(
							engine->asset_cache->add( std::make_unique<a_gradient>(), name, "" )
							);
					}

					// ------------------------------------------------------------------------

					asset_ptr->alignment = static_cast<e_align>( engine->find_int_from_symbol( iter_ad->find_value( "alignment" ) ) );

					asset_ptr->colors.clear();

					std::string color_list = iter_ad->find_value( "colors");

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
					filename = iter_ad->find_value( "filename");

					// ------------------------------------------------------------------------

					auto asset_ptr = engine->get_asset<a_font_def>( name, b_silent( true ) );

					if( !asset_ptr )
						asset_ptr = static_cast<a_font_def*>(
							engine->asset_cache->add( std::make_unique<a_font_def>(), name, filename )
							);

					// ------------------------------------------------------------------------

					asset_ptr->original_filename = filename;
					asset_ptr->texture_name = iter_ad->find_value( "texture");

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals( is_hot_reloading );
				}
				else if( type == "9slice_def" )
				{
					// ------------------------------------------------------------------------

					auto asset_ptr = engine->get_asset<a_9slice_def>( name, b_silent( true ) );

					if( !asset_ptr )
						asset_ptr = static_cast<a_9slice_def*>(
							engine->asset_cache->add( std::make_unique<a_9slice_def>(), name, filename )
							);

					// ------------------------------------------------------------------------

					std::string tex_name = iter_ad->find_value( "texture");

					for( const auto& iter : (*iter_ad->data()) )
					{
						std::string key = iter.first;
						std::string value = iter.second;

						int subtexture_idx = 0;

						if( key.substr( 0, 2 ) == "p_" )
						{
							subtexture_idx = w_parser::int_from_str( key );
							w_rect rc = w_parser::rect_from_str( value );

							std::string subtex_name = name + "_" + key;

							asset_ptr->patches[ subtexture_idx ] = static_cast<a_subtexture*>(
								engine->asset_cache->add(
									std::make_unique<a_subtexture>( tex_name, rc ), subtex_name, ""
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
					filename = iter_ad->find_value( "filename");

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
					filename = iter_ad->find_value( "filename");

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

					for( const auto& iter : (*iter_ad->data()) )
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
							asset_ptr->a_dir = w_parser::float_from_str( value );
						else if( key == "r_dir_var" )
							asset_ptr->r_dir_var = w_parser::range_from_str( value );
						else if( key == "r_scale_spawn" )
							asset_ptr->r_scale_spawn = w_parser::range_from_str( value );
						else if( key == "t_scale" )
							asset_ptr->t_scale = w_parser::timeline_from_str( e_timeline_type::float_type, value );
						else if( key == "s_max_spawn_per_sec" )
							asset_ptr->s_max_spawn_per_sec = w_parser::float_from_str( value );
						else if( key == "r_lifespan" )
							asset_ptr->r_lifespan = w_parser::range_from_str( value );
						else if( key == "r_velocity_spawn" )
							asset_ptr->r_velocity_spawn = w_parser::range_from_str( value );
						else if( key == "t_color" )
							asset_ptr->t_color = w_parser::timeline_from_str( e_timeline_type::color_type, value );
						else if( key == "r_spin_spawn" )
							asset_ptr->r_spin_spawn = w_parser::range_from_str( value );
						else if( key == "r_spin_per_sec" )
							asset_ptr->r_spin_per_sec = w_parser::range_from_str( value );
						else if( key == "t_alpha" )
							asset_ptr->t_alpha = w_parser::timeline_from_str( e_timeline_type::float_type, value );
						else
							log_msg( "%s : unknown key read from config block : [%s -> \"%s\"]", __FUNCTION__, name.c_str(), key.c_str() );
					}

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals( is_hot_reloading );
				}
				else if( type == "font" )
				{
					auto asset_ptr = engine->get_asset<a_font>( name, b_silent( true ) );

					if( !asset_ptr )
						asset_ptr = static_cast<a_font*>(
							engine->asset_cache->add( std::make_unique<a_font>(), name, "" )
							);

					// ------------------------------------------------------------------------

					asset_ptr->font_def = engine->get_asset<a_font_def>( iter_ad->find_value( "font_def") );

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals( is_hot_reloading );
				}
				else if( type == "cursor" )
				{
					auto asset_ptr = engine->get_asset<a_cursor>( name, b_silent( true ) );

					if( !asset_ptr )
						asset_ptr = static_cast<a_cursor*>(
							engine->asset_cache->add( std::make_unique<a_cursor>(), name, "" )
							);

					// ------------------------------------------------------------------------

					asset_ptr->img = engine->get_asset<a_subtexture>( iter_ad->find_value( "subtexture") );
					asset_ptr->hotspot_offset = w_parser::vec2_from_str( iter_ad->find_value( "hotspot") );

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals( is_hot_reloading );
				}
				else if( type == "anim_texture" )
				{
					auto asset_ptr = engine->get_asset<a_anim_texture>( name, b_silent( true ) );

					int frames_per_sec = w_parser::int_from_str( iter_ad->find_value( "frames_per_sec") );
					e_tween_type tween_type = static_cast<e_tween_type>( w_parser::int_from_str( iter_ad->find_value( "tween") ) );

					if( !asset_ptr )
					{
						asset_ptr = static_cast<a_anim_texture*>(
							engine->asset_cache->add(
								std::make_unique<a_anim_texture>( tween_type, frames_per_sec ),
								name, "" )
							);
					}

					// ------------------------------------------------------------------------

					const std::string frames = iter_ad->find_value( "frames");

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
					auto asset_ptr = engine->get_asset<a_subtexture>( name, b_silent( true ) );

					if( !asset_ptr )
					{
						w_rect rc = w_parser::rect_from_str( iter_ad->find_value( "rect") );
						asset_ptr = static_cast<a_subtexture*>(
							engine->asset_cache->add( std::make_unique<a_subtexture>( iter_ad->find_value( "texture"), rc ), name, "" )
							);
					}

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals( is_hot_reloading );
				}
				else if( type == "subtexture_list" )
				{
				}
			}
			break;
		}
	}
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
	std::unique_ptr<w_keyvalues> current_asset_definition = nullptr;

	// loop through every line of the asset_def fil and 
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

			// remove comments from the ends of lines, if they are there
			size_t pos = line.find_first_of( "#" );
			if( pos != std::string::npos )
			{
				line = w_stringutil::rtrim( line.substr( 0, pos ) );
			}

			// a "{" marks the beginning of a new asset definition
			if( line[ 0 ] == '{' )
			{
				current_asset_definition = std::make_unique<w_keyvalues>();
			}
			// a "}" marks the end of the current asset definition
			else if( line[0] == '}' )
			{
				asset_definitions.emplace_back( std::move( current_asset_definition ) );
				current_asset_definition = nullptr;
			}
			// parse each line into a key/value pair for the current asset definition
			else
			{
				w_tokenizer tok( line, '\"' );

				tok.get_next_token();
				std::string key = tok.get_next_token();
				tok.get_next_token();
				std::string value = tok.get_next_token();

				current_asset_definition->add( key, value );
			}
		}

		line = tok.get_next_token();
	}

	if( g_allow_hot_reload )
	{
		// save the last time modified for hot reloading
		std::scoped_lock lock( mutex_last_write_time );
		last_write_time = last_write_time_on_disk = retrieve_last_write_time_from_disk();

		if( is_hot_reloading )
			for( int p = 0; p < w_engine::num_asset_def_passes; ++p )
				precache_asset_resources( p, is_hot_reloading );
	}

	return true;
}
