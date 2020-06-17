#include "master_pch.h"
#include "master_header.h"

w_asset_definition_file::~w_asset_definition_file()
{
	clean_up_internals();
}

void w_asset_definition_file::precache_asset_resources( int pass_num, bool is_hot_reloading )
{
	std::string type, name, filename;

	for( auto& iter_ad : asset_definitions )
	{
		if( !iter_ad->key_values.count( "type" ) || !iter_ad->key_values.count( "name" ) )
			log_error( "%s : asset definition missing critical key/values", __FUNCTION__ );

		type = iter_ad->key_values["type"];
		name = iter_ad->key_values["name"];
		filename = "";

		if (pass_num == 0)
		{
			if (type == "preproc")
			{
				for (auto& iter : iter_ad->key_values)
				{
					std::string key = iter.first.c_str();
					std::string value = iter.second.c_str();

					if (key != "name" && key != "type")
						engine->_symbol_to_value[key] = value;
				}
			}
		}
		else if( pass_num == 1 )
		{
			if( type == "texture" )
			{
				if( !iter_ad->key_values.count( "filename" ) )
					log_error( "%s : '%s'.'%s' asset definition missing 'filename' key/value", type.c_str(), name.c_str(), __FUNCTION__ );

				filename = iter_ad->key_values["filename"];

				// ------------------------------------------------------------------------

				auto asset_ptr = engine->get_asset<a_texture>( name.c_str(), true );

				if( !asset_ptr )
					asset_ptr = static_cast<a_texture*>(
						engine->asset_cache->add( std::make_unique<a_texture>(), name.c_str(), filename.c_str() )
					);

				// ------------------------------------------------------------------------

				asset_ptr->original_filename = filename;

				// ------------------------------------------------------------------------

				asset_ptr->clean_up_internals();
				asset_ptr->create_internals( is_hot_reloading );
			}
			else if( type == "gradient" )
			{
				if( !iter_ad->key_values.count( "alignment" ) )
					log_error( "%s : '%s'.'%s' asset definition missing 'alignment' key/value", type.c_str(), name.c_str(), __FUNCTION__ );

				if( !iter_ad->key_values.count( "colors" ) )
					log_error( "%s : '%s'.'%s' asset definition missing 'colors' key/value", type.c_str(), name.c_str(), __FUNCTION__ );

				// ------------------------------------------------------------------------

				auto asset_ptr = engine->get_asset<a_gradient>( name.c_str(), true );

				if( !asset_ptr )
					asset_ptr = static_cast<a_gradient*>(
						engine->asset_cache->add( std::make_unique<a_gradient>(), name.c_str(), "" )
					);

				// ------------------------------------------------------------------------

				asset_ptr->alignment = static_cast<e_align>( engine->find_int_from_symbol( iter_ad->key_values["alignment"] ) );
				
				asset_ptr->colors.clear();

				std::string color_list = iter_ad->key_values["colors"];

				w_tokenizer tok( color_list, '/', "n/a" );
				std::string val;

				std::vector<std::string> color_values;
				while( true )
				{
					val = tok.get_next_token();
					if( val == "n/a" )
						break;

					color_values.push_back( w_stringutil::trim( val ) );
				}

				for( auto& iter : color_values )
				{
					val = iter;

					if( engine->is_symbol_in_map( val ) )
						val = engine->find_val_from_symbol( val );

					asset_ptr->colors.push_back( w_color( val ) );
				}

				// ------------------------------------------------------------------------

				asset_ptr->clean_up_internals();
				asset_ptr->create_internals( is_hot_reloading );
			}
			else if( type == "font_def" )
			{
				if( !iter_ad->key_values.count( "filename" ) )
					log_error( "%s : '%s'.'%s' asset definition missing 'filename' key/value", type.c_str(), name.c_str(), __FUNCTION__ );
				if( !iter_ad->key_values.count( "texture" ) )
					log_error( "%s : '%s'.'%s' asset definition missing 'texture' key/value", type.c_str(), name.c_str(), __FUNCTION__ );

				filename = iter_ad->key_values["filename"];

				// ------------------------------------------------------------------------

				auto asset_ptr = engine->get_asset<a_font_def>( name.c_str(), true );

				if( !asset_ptr  )
					asset_ptr = static_cast<a_font_def*>(
						engine->asset_cache->add( std::make_unique<a_font_def>(), name.c_str(), filename.c_str() )
					);

				// ------------------------------------------------------------------------

				asset_ptr->original_filename = filename;
				asset_ptr->texture_name = iter_ad->key_values[ "texture" ];

				// ------------------------------------------------------------------------

				asset_ptr->clean_up_internals();
				asset_ptr->create_internals( is_hot_reloading );
			}
			else if( type == "atlas_def" )
			{
				if( !iter_ad->key_values.count( "filename" ) )
					log_error( "%s : '%s'.'%s' asset definition missing 'filename' key/value", type.c_str(), name.c_str(), __FUNCTION__ );

				filename = iter_ad->key_values[ "filename" ];

				// ------------------------------------------------------------------------

				auto asset_ptr = engine->get_asset<a_atlas_def>( name.c_str(), true );

				if( !asset_ptr )
					asset_ptr = static_cast<a_atlas_def*>(
						engine->asset_cache->add( std::make_unique<a_atlas_def>(), name.c_str(), filename.c_str() )
						);

				// ------------------------------------------------------------------------

				asset_ptr->original_filename = filename;

				// ------------------------------------------------------------------------

				asset_ptr->clean_up_internals();
				asset_ptr->create_internals( is_hot_reloading );
			}
			else if( type == "9slice_def" )
			{
				// ------------------------------------------------------------------------

				auto asset_ptr = engine->get_asset<a_9slice_def>( name.c_str(), true );

				if( !asset_ptr )
					asset_ptr = static_cast<a_9slice_def*>(
						engine->asset_cache->add( std::make_unique<a_9slice_def>(), name.c_str(), filename.c_str() )
					);

				// ------------------------------------------------------------------------

				for( auto& iter : iter_ad->key_values )
				{
					std::string key = iter.first.c_str();
					std::string value = iter.second.c_str();

					if( key.substr( 0, 6 ) == "patch_" )
					{
						int patch_idx;

						if( key == "patch_00" )			patch_idx = (int)e_patch::P_00;
						else if( key == "patch_10" )	patch_idx = (int)e_patch::P_10;
						else if( key == "patch_20" )	patch_idx = (int)e_patch::P_20;
						else if( key == "patch_01" )	patch_idx = (int)e_patch::P_01;
						else if( key == "patch_11" )	patch_idx = (int)e_patch::P_11;
						else if( key == "patch_21" )	patch_idx = (int)e_patch::P_21;
						else if( key == "patch_02" )	patch_idx = (int)e_patch::P_02;
						else if( key == "patch_12" )	patch_idx = (int)e_patch::P_12;
						else if( key == "patch_22" )	patch_idx = (int)e_patch::P_22;

						w_tokenizer tok( value, ',' );
						asset_ptr->patches[patch_idx].x = STRTOF( float, tok.get_next_token() );
						asset_ptr->patches[patch_idx].y = STRTOF( float, tok.get_next_token() );
						asset_ptr->patches[patch_idx].w = STRTOF( float, tok.get_next_token() );
						asset_ptr->patches[patch_idx].h = STRTOF( float, tok.get_next_token() );
					}
				}

				// ------------------------------------------------------------------------

				asset_ptr->clean_up_internals();
				asset_ptr->create_internals( is_hot_reloading );
			}
			else if( type == "sound" )
			{
				if( !iter_ad->key_values.count( "filename" ) )
					log_error( "%s : '%s'.'%s' asset definition missing 'filename' key/value", type.c_str(), name.c_str(), __FUNCTION__ );

				filename = iter_ad->key_values["filename"];

				// ------------------------------------------------------------------------

				auto asset_ptr = engine->get_asset<a_sound>( name.c_str(), true );

				if( !asset_ptr )
					asset_ptr = static_cast<a_sound*>(
						engine->asset_cache->add( std::make_unique<a_sound>(), name.c_str(), filename.c_str() )
					);

				// ------------------------------------------------------------------------

				asset_ptr->original_filename = filename;

				// ------------------------------------------------------------------------

				asset_ptr->clean_up_internals();
				asset_ptr->create_internals( is_hot_reloading );
			}
			else if( type == "music" )
			{
				if( !iter_ad->key_values.count( "filename" ) )
					log_error( "%s : '%s'.'%s' asset definition missing 'filename' key/value", type.c_str(), name.c_str(), __FUNCTION__ );

				filename = iter_ad->key_values["filename"];

				// ------------------------------------------------------------------------

				auto asset_ptr = engine->get_asset<a_music>( name.c_str(), true );

				if( !asset_ptr )
					asset_ptr = static_cast<a_music*>(
						engine->asset_cache->add( std::make_unique<a_music>(), name.c_str(), filename.c_str() )
					);

				// ------------------------------------------------------------------------

				asset_ptr->original_filename = filename;

				// ------------------------------------------------------------------------

				asset_ptr->clean_up_internals();
				asset_ptr->create_internals( is_hot_reloading );
			}
		}
		else if( pass_num == 2 )
		{
			if( type == "emitter_params" )
			{
				// ------------------------------------------------------------------------

				auto asset_ptr = engine->get_asset<a_emitter_params>( name.c_str(), true );

				if( !asset_ptr )
					asset_ptr = static_cast<a_emitter_params*>(
						engine->asset_cache->add( std::make_unique<a_emitter_params>(), name.c_str(), "" )
					);

				// ------------------------------------------------------------------------

				for( auto& iter : iter_ad->key_values )
				{
					std::string key = iter.first.c_str();
					std::string value = iter.second.c_str();

					if( key == "name" || key == "type" )
					{
						// these are valid, we just don't need them here
					}
					else if( key == "b_needs_warm_up" )
						asset_ptr->b_needs_warm_up = ( value == "true" ) ? true : false;
					else if( key == "b_one_shot" )
						asset_ptr->b_one_shot = ( value == "true" ) ? true : false;
					else if( key == "texture_name" )
						asset_ptr->tex = engine->get_asset<a_texture>( value.c_str() );
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
				if( !iter_ad->key_values.count( "texture" ) )
					log_error( "%s : '%s'.'%s' asset definition missing 'texture' key/value", type.c_str(), name.c_str(), __FUNCTION__ );

				if( !iter_ad->key_values.count( "font_def" ) )
					log_error( "%s : '%s'.'%s' asset definition missing 'font_def' key/value", type.c_str(), name.c_str(), __FUNCTION__ );

				// ------------------------------------------------------------------------

				auto asset_ptr = engine->get_asset<a_font>( name.c_str(), true );

				if( !asset_ptr )
					asset_ptr = static_cast<a_font*>(
						engine->asset_cache->add( std::make_unique<a_font>(), name.c_str(), "" )
					);

				// ------------------------------------------------------------------------

				asset_ptr->tex = engine->get_asset<a_texture>( iter_ad->key_values["texture"].c_str() );
				asset_ptr->font_def = engine->get_asset<a_font_def>( iter_ad->key_values["font_def"].c_str() );

				// ------------------------------------------------------------------------

				asset_ptr->clean_up_internals();
				asset_ptr->create_internals( is_hot_reloading );
			}
			else if( type == "anim_texture" )
			{
				auto asset_ptr = engine->get_asset<a_anim_texture>( name.c_str(), true );

				if( !asset_ptr )
					asset_ptr = static_cast<a_anim_texture*>(
						engine->asset_cache->add( std::make_unique<a_anim_texture>(), name.c_str(), "" )
					);

				// ------------------------------------------------------------------------

				std::string frames = iter_ad->key_values["frames"];
				size_t num_frames = std::count( frames.begin(), frames.end(), ',' ) + 1;

				w_tokenizer tok( frames, ',' );
				for( ; num_frames > 0 ; --num_frames )
				{
					auto tex = engine->get_asset<a_texture>( tok.get_next_token().c_str() );
					asset_ptr->frames.push_back( tex );
				}

				// ------------------------------------------------------------------------

				asset_ptr->clean_up_internals();
				asset_ptr->create_internals( is_hot_reloading );
			}
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
	std::unique_ptr<w_asset_definition> current_asset_definition = nullptr;

	while( !tok.is_eos() )
	{
		line = w_stringutil::trim( line );

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
				asset_definitions.push_back( std::move( current_asset_definition ) );
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
