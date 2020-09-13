#include "master_pch.h"
#include "master_header.h"

void w_asset_definition_file::precache_asset_resources( size_t pass_num )
{
	std::string type, name, filename;

	std::filesystem::path filename_path = original_filename;
	std::string data_folder = filename_path.parent_path().generic_string();
	data_folder += "/";

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
					for( auto& [key,value] : iter_ad->kv )
					{
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
					filename = fmt::format( "{}{}", data_folder, iter_ad->find_value( "filename" ) );

					// ------------------------------------------------------------------------

					auto asset_ptr = engine->get_asset<a_texture>( name, b_silent(true) );

					if( !asset_ptr )
					{
						asset_ptr = engine->asset_cache->add( std::make_unique<a_texture>(), name, filename );
					}

					// ------------------------------------------------------------------------

					asset_ptr->original_filename = filename;

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals();

					// every texture has to have a subtexture defined for it.
					//
					// you may specify the name of this subtexture by including the...
					// "subtexture" "subtex_name"
					// ...k/v pair in the texture definition.
					//
					// if that k/v pair isn't there, "sub_" will be prepended to
					// the texture name.

					std::string subtex_name = std::string( iter_ad->find_value_opt( "subtexture", "sub_" + name ) );

					auto subtex = engine->asset_cache->add( std::make_unique<a_subtexture>( name ), subtex_name, "" );

					asset_ptr->subtex = subtex;

					// the "subtextures" k/v is a convenient way to specify a set
					// of subtextures belonging to a texture. an easy way to break
					// down atlas textures or sprite sheets into subtextures.

					if( iter_ad->does_key_exist( "subtextures" ) )
					{
						std::string_view subtex_list = iter_ad->find_value( "subtextures" );

						int comma_count = static_cast<int>( std::count( subtex_list.begin(), subtex_list.end(), ',' ) );

						if( ( comma_count % 5 ) != 4 )
							log_error( "'{}' has bad formatting - too many or too few commas", subtex_list );

						w_tokenizer tok( subtex_list, ',' );

						while( !tok.is_eos() )
						{
							std::string_view subtex_name = *tok.get_next_token();

							float x = w_parser::float_from_str( *tok.get_next_token() );
							float y = w_parser::float_from_str( *tok.get_next_token() );
							float w = w_parser::float_from_str( *tok.get_next_token() );
							float h = w_parser::float_from_str( *tok.get_next_token() );

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
						asset_ptr = engine->asset_cache->add( std::make_unique<a_gradient>(), name, "" );
					}

					// ------------------------------------------------------------------------

					asset_ptr->alignment = static_cast<e_align>( engine->find_int_from_symbol( iter_ad->find_value( "alignment" ) ) );

					asset_ptr->colors.clear();

					std::string color_list = std::string( iter_ad->find_value( "colors" ) );

					w_tokenizer tok( color_list, ',' );

					std::vector<std::string> wk_values;
					while( true )
					{
						auto val = tok.get_next_token();

						if( !val.has_value() )
						{
							break;
						}

						char ch = ( *val )[ 0 ];
						if( (ch >= '0' && ch <= '9') || ch == '.' )
						{
							std::string composited_color = std::string( *val );
							composited_color += ",";
							composited_color += *tok.get_next_token();
							composited_color += ",";
							composited_color += *tok.get_next_token();

							wk_values.push_back( composited_color );
						}
						else
						{
							wk_values.push_back( std::string( *val ) );
						}
					}

					std::vector<std::string> color_values;
					int repeat_count;
					for( auto& color_value : wk_values )
					{
						repeat_count = 1;

						size_t pos = color_value.find_last_of( ':' );
						if( pos != std::string::npos )
						{
							pos++;
							repeat_count = strtol( color_value.substr( pos, color_value.length() - pos ).data(), ( char** ) nullptr, 10 );
							color_value = color_value.substr( 0, pos - 1 );
						}

						while( repeat_count > 0 )
						{
							color_values.emplace_back( color_value );
							repeat_count--;
						}
					}

					// must reverse the order or else the gradient texture
					// ends up backwards on the screen
					std::reverse( color_values.begin(), color_values.end() );

					for( const auto& iter : color_values )
					{
						auto val = iter;

						if( engine->is_symbol_in_map( val ) )
						{
							val = *engine->find_string_from_symbol( val );
						}

						auto color = w_color( val );
						asset_ptr->colors.push_back( color.r );
						asset_ptr->colors.push_back( color.g );
						asset_ptr->colors.push_back( color.b );
						asset_ptr->colors.push_back( 1.0f );
					}

					// if the "subtexture" key exists, create a subtexture for this texture
					// that represents it's entirety.

					if( iter_ad->does_key_exist( "subtexture" ) )
					{
						auto subtex = engine->asset_cache->add( std::make_unique<a_subtexture>( name ),
																iter_ad->find_value( "subtexture" ), "" );
						asset_ptr->subtex = subtex;
					}

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals();
				}
				else if( type == "font_def" )
				{
					filename = fmt::format( "{}{}", data_folder, iter_ad->find_value( "filename" ) );

					// ------------------------------------------------------------------------

					auto asset_ptr = engine->get_asset<a_font_def>( name, b_silent( true ) );

					if( !asset_ptr )
					{
						asset_ptr = engine->asset_cache->add( std::make_unique<a_font_def>(), name, filename );
					}

					// ------------------------------------------------------------------------

					asset_ptr->original_filename = filename;
					asset_ptr->texture = engine->get_asset<a_texture>( iter_ad->find_value( "texture" ) );

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals();
				}
				else if( type == "slice_def" )
				{
					// ------------------------------------------------------------------------

					auto asset_ptr = engine->get_asset<a_9slice_def>( name, b_silent( true ) );

					if( !asset_ptr )
					{
						asset_ptr = engine->asset_cache->add( std::make_unique<a_9slice_def>(), name, filename );
					}

					auto fill_mode = iter_ad->find_value_opt( "fill_mode" );
					asset_ptr->fill_mode = engine->find_int_from_symbol( fill_mode, fill_mode::scaled );

					// ------------------------------------------------------------------------

					std::string_view tex_name = iter_ad->find_value( "texture");

					std::optional<w_rect> rect;
					std::optional<w_vec2> x_slices;
					std::optional<w_vec2> y_slices;

					for( const auto& [key, value] : iter_ad->kv )
					{
						int subtex_idx = 0;
						w_rect rc = {};

						if( key == "rect" )
						{
							rect = w_parser::rect_from_str( value );
						}
						else if( key == "x_slices" )
						{
							x_slices = w_parser::vec2_from_str( value );
						}
						else if( key == "y_slices" )
						{
							y_slices = w_parser::vec2_from_str( value );
						}
					}

					if( !rect || !x_slices || !y_slices )
					{
						log_error( "Malformed slice definition : {}", name );
					}

					float x, y, w, h;

					// top row

					x = rect->x;
					y = rect->y;
					w = x_slices->left;
					h = y_slices->top;

					asset_ptr->patches[ slicedef_patch::P_00 ] =
						engine->asset_cache->add(
							std::make_unique<a_subtexture>( tex_name, w_rect( x, y, w, h ) ), fmt::format( "sub_{}_00", name ), ""
						);

					x = rect->x + x_slices->left;
					w = *rect->w - x_slices->left - x_slices->right;
					asset_ptr->patches[ slicedef_patch::P_10 ] =
						engine->asset_cache->add(
							std::make_unique<a_subtexture>( tex_name, w_rect( x, y, w, h ) ), fmt::format( "sub_{}_10", name ), ""
						);

					x = rect->x + *rect->w - x_slices->right;
					w = x_slices->right;
					asset_ptr->patches[ slicedef_patch::P_20 ] =
						engine->asset_cache->add(
							std::make_unique<a_subtexture>( tex_name, w_rect( x, y, w, h ) ), fmt::format( "sub_{}_20", name ), ""
						);

					// middle row

					x = rect->x;
					y = rect->y + y_slices->top;
					w = x_slices->left;
					h = *rect->h - y_slices->top - y_slices->bottom;

					asset_ptr->patches[ slicedef_patch::P_01 ] =
						engine->asset_cache->add(
							std::make_unique<a_subtexture>( tex_name, w_rect( x, y, w, h ) ), fmt::format( "sub_{}_01", name ), ""
						);

					x = rect->x + x_slices->left;
					w = *rect->w - x_slices->left - x_slices->right;
					asset_ptr->patches[ slicedef_patch::P_11 ] =
						engine->asset_cache->add(
							std::make_unique<a_subtexture>( tex_name, w_rect( x, y, w, h ) ), fmt::format( "sub_{}_11", name ), ""
						);

					x = rect->x + *rect->w - x_slices->right;
					w = x_slices->right;
					asset_ptr->patches[ slicedef_patch::P_21 ] =
						engine->asset_cache->add(
							std::make_unique<a_subtexture>( tex_name, w_rect( x, y, w, h ) ), fmt::format( "sub_{}_21", name ), ""
						);

					// bottom row

					x = rect->x;
					y = rect->y + *rect->h - y_slices->bottom;
					w = x_slices->left;
					h = y_slices->bottom;

					asset_ptr->patches[ slicedef_patch::P_02 ] =
						engine->asset_cache->add(
							std::make_unique<a_subtexture>( tex_name, w_rect( x, y, w, h ) ), fmt::format( "sub_{}_02", name ), ""
						);

					x = rect->x + x_slices->left;
					w = *rect->w - x_slices->left - x_slices->right;
					asset_ptr->patches[ slicedef_patch::P_12 ] =
						engine->asset_cache->add(
							std::make_unique<a_subtexture>( tex_name, w_rect( x, y, w, h ) ), fmt::format( "sub_{}_12", name ), ""
						);

					x = rect->x + *rect->w - x_slices->right;
					w = x_slices->right;
					asset_ptr->patches[ slicedef_patch::P_22 ] =
						engine->asset_cache->add(
							std::make_unique<a_subtexture>( tex_name, w_rect( x, y, w, h ) ), fmt::format( "sub_{}_22", name ), ""
						);

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals();
				}
				else if( type == "sound" )
				{
					filename = fmt::format( "{}{}", data_folder, iter_ad->find_value( "filename" ) );

					// ------------------------------------------------------------------------

					auto asset_ptr = engine->get_asset<a_sound>( name, b_silent( true ) );

					if( !asset_ptr )
					{
						asset_ptr = engine->asset_cache->add( std::make_unique<a_sound>(), name, filename );
					}

					// ------------------------------------------------------------------------

					asset_ptr->original_filename = filename;

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals();
				}
				else if( type == "music" )
				{
					filename = fmt::format( "{}{}", data_folder, iter_ad->find_value( "filename" ) );

					// ------------------------------------------------------------------------

					auto asset_ptr = engine->get_asset<a_music>( name, b_silent( true ) );

					if( !asset_ptr )
					{
						asset_ptr = engine->asset_cache->add( std::make_unique<a_music>(), name, filename );
					}

					// ------------------------------------------------------------------------

					asset_ptr->original_filename = filename;

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals();
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
					{
						asset_ptr = engine->asset_cache->add( std::make_unique<a_emitter_params>(), name, "" );
					}

					// ------------------------------------------------------------------------

					for( const auto& [key, value] : iter_ad->kv )
					{
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
							std::string_view type = *tok.get_next_token();

							if( type == "point" )
							{
								// the default spawner type, don't need to do anything for this
							}
							else if( type == "box" )
							{
								asset_ptr->particle_spawner = std::make_unique<w_particle_spawner_box>();
							}
							else if( type == "circle" )
							{
								asset_ptr->particle_spawner = std::make_unique<w_particle_spawner_circle>();
							}
							else
							{
								log_error( "Unknown emitter spawn type : [{}]", type );
							}

							asset_ptr->particle_spawner->parse_from_config_string( value );
						}
						else if( key == "a_dir" )
							asset_ptr->a_dir = w_parser::float_from_str( value );
						else if( key == "r_dir_var" )
							asset_ptr->r_dir_var = w_parser::range_from_str( value );
						else if( key == "r_scale_spawn" )
							asset_ptr->r_scale_spawn = w_parser::range_from_str( value );
						else if( key == "t_scale" )
							asset_ptr->t_scale = w_parser::timeline_from_str( timeline_type::float_type, value );
						else if( key == "s_max_spawn_per_sec" )
							asset_ptr->s_max_spawn_per_sec = w_parser::float_from_str( value );
						else if( key == "r_lifespan" )
							asset_ptr->r_lifespan = w_parser::range_from_str( value );
						else if( key == "r_velocity_spawn" )
							asset_ptr->r_velocity_spawn = w_parser::range_from_str( value );
						else if( key == "t_color" )
							asset_ptr->t_color = w_parser::timeline_from_str( timeline_type::color_type, value );
						else if( key == "r_spin_spawn" )
							asset_ptr->r_spin_spawn = w_parser::range_from_str( value );
						else if( key == "r_spin_per_sec" )
							asset_ptr->r_spin_per_sec = w_parser::range_from_str( value );
						else if( key == "t_alpha" )
							asset_ptr->t_alpha = w_parser::timeline_from_str( timeline_type::float_type, value );
						else
							log_msg( "Unknown key read from config block : [{} -> \"{}\"]", name, key );
					}

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals();
				}
				else if( type == "font" )
				{
					auto asset_ptr = engine->get_asset<a_font>( name, b_silent( true ) );

					if( !asset_ptr )
					{
						asset_ptr = engine->asset_cache->add( std::make_unique<a_font>(), name, "" );
					}

					// ------------------------------------------------------------------------

					asset_ptr->font_def = engine->get_asset<a_font_def>( iter_ad->find_value( "font_def") );

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals();
				}
				else if( type == "cursor" )
				{
					auto asset_ptr = engine->get_asset<a_cursor>( name, b_silent( true ) );

					if( !asset_ptr )
					{
						asset_ptr = engine->asset_cache->add( std::make_unique<a_cursor>(), name, "" );
					}

					// ------------------------------------------------------------------------

					asset_ptr->subtex = engine->get_asset<a_subtexture>( iter_ad->find_value( "subtexture") );
					asset_ptr->hotspot_offset = w_parser::vec2_from_str( iter_ad->find_value( "hotspot") );

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals();
				}
				else if( type == "anim_texture" )
				{
					auto asset_ptr = engine->get_asset<a_anim_texture>( name, b_silent( true ) );

					int frames_per_sec = w_parser::int_from_str( iter_ad->find_value( "frames_per_sec") );
					auto tween_type = static_cast<e_tween_type>( w_parser::int_from_str( iter_ad->find_value( "tween") ) );

					if( !asset_ptr )
					{
						asset_ptr = engine->asset_cache->add( std::make_unique<a_anim_texture>( tween_type, frames_per_sec ),
															  name, "" );
					}

					// ------------------------------------------------------------------------

					const std::string_view frames = iter_ad->find_value( "frames");

					w_tokenizer tok( frames, ',' );
					while( !tok.is_eos() )
					{
						auto tex = engine->get_asset<a_subtexture>( *tok.get_next_token() );
						asset_ptr->frames.emplace_back( tex );
					}

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals();
				}
				else if( type == "subtexture" )
				{
					auto asset_ptr = engine->get_asset<a_subtexture>( name, b_silent( true ) );

					if( !asset_ptr )
					{
						w_rect rc = w_parser::rect_from_str( iter_ad->find_value( "rect") );
						asset_ptr = engine->asset_cache->add( std::make_unique<a_subtexture>( iter_ad->find_value( "texture" ), rc ),
															  name, "" );
					}

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals();
				}
			}
			break;
		}
	}
}

void w_asset_definition_file::clean_up_internals()
{
}

bool w_asset_definition_file::create_internals()
{
	asset_definitions.clear();

	// read the contents of the asset definition file and break it up
	// into individual asset definitions

	auto file = engine->fs->load_text_file_into_memory( original_filename );

	std::unique_ptr<w_keyvalues> current_asset_definition = nullptr;

	// loop through every line of the asset_def fil and
	for( const auto& line : *( file.get()->lines.get() ) )
	{
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
			w_tokenizer tok_kv( line, '\"' );

			tok_kv.get_next_token();
			auto key = tok_kv.get_next_token();
			tok_kv.get_next_token();
			auto value = tok_kv.get_next_token();

			if( key.has_value() && value.has_value() )
			{
				current_asset_definition->set( *key, *value );
			}
		}
	}

	return true;
}
