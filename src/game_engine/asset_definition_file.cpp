#include "master_pch.h"
#include "master_header.h"

void w_asset_definition_file::precache_asset_resources( size_t pass_num )
{
	std::string type, tag, filename;

	std::filesystem::path filename_path = original_filename;
	std::string data_folder = filename_path.parent_path().generic_string();
	data_folder += "/";

	for( const auto& iter_ad : asset_definitions )
	{
		if( !iter_ad->kv.size() )
		{
			continue;
		}

		type = iter_ad->find_value( "type" );
		tag = iter_ad->find_value( "tag" );
		filename = "";

		switch( pass_num )
		{
			case 0:
			{
				if( type == "preproc" )
				{
					for( auto& [key,value] : iter_ad->kv )
					{
						if( key != "tag" && key != "type" )
						{
							engine->_symbol_to_value[ key ] = value;
						}
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

					auto asset_ptr = a_texture::find( tag, b_silent(true) );

					if( !asset_ptr )
					{
						asset_ptr = engine->asset_cache->add( std::make_unique<a_texture>(), tag, filename );
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

					std::string subtex_name = std::string( iter_ad->find_value_opt( "subtexture", "sub_" + tag ) );

					auto subtex = engine->asset_cache->add( std::make_unique<a_subtexture>( tag ), subtex_name, "" );

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
							subtex_name = std::string( *tok.get_next_token() );

							float x = w_parser::float_from_str( *tok.get_next_token() );
							float y = w_parser::float_from_str( *tok.get_next_token() );
							float w = w_parser::float_from_str( *tok.get_next_token() );
							float h = w_parser::float_from_str( *tok.get_next_token() );

							w_rect rc( x, y, w, h );

							engine->asset_cache->add( std::make_unique<a_subtexture>( tag, rc ), subtex_name, "" );
						}
					}
				}
				else if( type == "gradient" )
				{
					// ------------------------------------------------------------------------

					auto asset_ptr = a_gradient::find( tag, b_silent( true ) );

					if( !asset_ptr )
					{
						asset_ptr = engine->asset_cache->add( std::make_unique<a_gradient>(), tag, "" );
					}

					// ------------------------------------------------------------------------

					asset_ptr->alignment = static_cast<e_align>( engine->find_int_from_symbol( iter_ad->find_value( "alignment" ) ) );

					asset_ptr->colors = {};

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
							wk_values.emplace_back( std::string( *val ) );
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
							repeat_count = str_to_int( color_value.substr( pos, color_value.length() - pos ) );
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
						auto subtex = engine->asset_cache->add( std::make_unique<a_subtexture>( tag ),
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

					auto asset_ptr = a_font_def::find( tag, b_silent( true ) );

					if( !asset_ptr )
					{
						asset_ptr = engine->asset_cache->add( std::make_unique<a_font_def>(), tag, filename );
					}

					// ------------------------------------------------------------------------

					asset_ptr->original_filename = filename;
					asset_ptr->texture = a_texture::find( iter_ad->find_value( "texture" ) );

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals();
				}
				else if( type == "slice_def" )
				{
					// ------------------------------------------------------------------------

					auto asset_ptr = a_9slice_def::find( tag, b_silent( true ) );

					if( !asset_ptr )
					{
						asset_ptr = engine->asset_cache->add( std::make_unique<a_9slice_def>(), tag, filename );
					}

					// ------------------------------------------------------------------------

					a_subtexture* subtexture = nullptr;

					std::optional<std::string_view> tex_tag = std::nullopt;
					if( iter_ad->does_key_exist("texture") )
					{
						tex_tag = iter_ad->find_value( "texture" );
						subtexture = a_texture::find( *tex_tag )->get_subtexture();
					}

					std::optional<std::string_view> sub_tex_tag = std::nullopt;
					if( iter_ad->does_key_exist( "subtexture" ) )
					{
						sub_tex_tag = iter_ad->find_value( "subtexture" );
						subtexture = a_subtexture::find( *sub_tex_tag );
						tex_tag = subtexture->tex->tag;
					}

					std::optional<w_rect> rect;
					std::optional<w_vec2> x_slices;
					std::optional<w_vec2> y_slices;

					for( const auto& [key, value] : iter_ad->kv )
					{
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

					// No rectangle was specified but we have a subtexture name. use the
					// dimensions of that subtexture for the rect.
					if( !rect && sub_tex_tag )
					{
						rect = subtexture->get_bounding_rect();
					}

					if( !rect || !x_slices || !y_slices )
					{
						log_error( "Malformed slice definition : {}", tag );
					}

					float x, y, w, h;

					// top row

					x = rect->x;
					y = rect->y;
					w = x_slices->_left;
					h = y_slices->_top;

					asset_ptr->patches[ slicedef_patch::P_00 ] =
						engine->asset_cache->add(
							std::make_unique<a_subtexture>( *tex_tag, w_rect( x, y, w, h ) ), fmt::format( "sub_{}_00", tag ), ""
						);

					x = rect->x + x_slices->_left;
					w = rect->w - x_slices->_left - x_slices->_right;
					asset_ptr->patches[ slicedef_patch::P_10 ] =
						engine->asset_cache->add(
							std::make_unique<a_subtexture>( *tex_tag, w_rect( x, y, w, h ) ), fmt::format( "sub_{}_10", tag ), ""
						);

					x = rect->x + rect->w - x_slices->_right;
					w = x_slices->_right;
					asset_ptr->patches[ slicedef_patch::P_20 ] =
						engine->asset_cache->add(
							std::make_unique<a_subtexture>( *tex_tag, w_rect( x, y, w, h ) ), fmt::format( "sub_{}_20", tag ), ""
						);

					// middle row

					x = rect->x;
					y = rect->y + y_slices->_top;
					w = x_slices->_left;
					h = rect->h - y_slices->_top - y_slices->_bottom;

					asset_ptr->patches[ slicedef_patch::P_01 ] =
						engine->asset_cache->add(
							std::make_unique<a_subtexture>( *tex_tag, w_rect( x, y, w, h ) ), fmt::format( "sub_{}_01", tag ), ""
						);

					x = rect->x + x_slices->_left;
					w = rect->w - x_slices->_left - x_slices->_right;
					asset_ptr->patches[ slicedef_patch::P_11 ] =
						engine->asset_cache->add(
							std::make_unique<a_subtexture>( *tex_tag, w_rect( x, y, w, h ) ), fmt::format( "sub_{}_11", tag ), ""
						);

					x = rect->x + rect->w - x_slices->_right;
					w = x_slices->_right;
					asset_ptr->patches[ slicedef_patch::P_21 ] =
						engine->asset_cache->add(
							std::make_unique<a_subtexture>( *tex_tag, w_rect( x, y, w, h ) ), fmt::format( "sub_{}_21", tag ), ""
						);

					// bottom row

					x = rect->x;
					y = rect->y + rect->h - y_slices->_bottom;
					w = x_slices->_left;
					h = y_slices->_bottom;

					asset_ptr->patches[ slicedef_patch::P_02 ] =
						engine->asset_cache->add(
							std::make_unique<a_subtexture>( *tex_tag, w_rect( x, y, w, h ) ), fmt::format( "sub_{}_02", tag ), ""
						);

					x = rect->x + x_slices->_left;
					w = rect->w - x_slices->_left - x_slices->_right;
					asset_ptr->patches[ slicedef_patch::P_12 ] =
						engine->asset_cache->add(
							std::make_unique<a_subtexture>( *tex_tag, w_rect( x, y, w, h ) ), fmt::format( "sub_{}_12", tag ), ""
						);

					x = rect->x + rect->w - x_slices->_right;
					w = x_slices->_right;
					asset_ptr->patches[ slicedef_patch::P_22 ] =
						engine->asset_cache->add(
							std::make_unique<a_subtexture>( *tex_tag, w_rect( x, y, w, h ) ), fmt::format( "sub_{}_22", tag ), ""
						);

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals();
				}
				else if( type == "sound" )
				{
					filename = fmt::format( "{}{}", data_folder, iter_ad->find_value( "filename" ) );

					// ------------------------------------------------------------------------

					auto asset_ptr = a_sound::find( tag, b_silent( true ) );

					if( !asset_ptr )
					{
						asset_ptr = engine->asset_cache->add( std::make_unique<a_sound>(), tag, filename );
					}

					// ------------------------------------------------------------------------

					asset_ptr->original_filename = filename;

					for( auto& [key, value] : iter_ad->kv )
					{
						if( key == "looped" )
						{
							asset_ptr->looped = bool( value == "true" );
						}
					}

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

					auto asset_ptr = a_emitter_params::find( tag, b_silent( true ) );

					if( !asset_ptr )
					{
						asset_ptr = engine->asset_cache->add( std::make_unique<a_emitter_params>(), tag, "" );
					}

					// ------------------------------------------------------------------------

					for( const auto& [key, value] : iter_ad->kv )
					{
						if( key == "tag" || key == "type" )
						{
							// these are valid, we just don't need them here
						}
						else if( key == "b_needs_warm_up" )
							asset_ptr->needs_warm_up = bool( value == "true" );
						else if( key == "b_one_shot" )
							asset_ptr->is_one_shot = bool( value == "true" );
						else if( key == "texture_name" )
							asset_ptr->tex = a_texture::find( value );
						else if( key == "spawner_type" )
						{
							w_tokenizer tok( value, ',' );
							std::string spawner_type = std::string( *tok.get_next_token() );

							if( spawner_type == "point" )
							{
								// the default spawner type, don't need to do anything for this
							}
							else if( spawner_type == "box" )
							{
								asset_ptr->particle_spawner = std::make_unique<w_particle_spawner_box>();
							}
							else if( spawner_type == "circle" )
							{
								asset_ptr->particle_spawner = std::make_unique<w_particle_spawner_circle>();
							}
							else
							{
								log_error( "Unknown emitter spawn type : [{}]", spawner_type );
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
							log_msg( "Unknown key read from config block : [{} -> \"{}\"]", tag, key );
					}

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals();
				}
				else if( type == "font" )
				{
					auto asset_ptr = a_font::find( tag, b_silent( true ) );

					if( !asset_ptr )
					{
						asset_ptr = engine->asset_cache->add( std::make_unique<a_font>(), tag, "" );
					}

					// ------------------------------------------------------------------------

					asset_ptr->font_def = a_font_def::find( iter_ad->find_value( "font_def") );

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals();
				}
				else if( type == "cursor" )
				{
					auto asset_ptr = a_cursor::find( tag, b_silent( true ) );

					if( !asset_ptr )
					{
						asset_ptr = engine->asset_cache->add( std::make_unique<a_cursor>(), tag, "" );
					}

					// ------------------------------------------------------------------------

					asset_ptr->subtex = a_subtexture::find( iter_ad->find_value( "subtexture") );
					asset_ptr->hotspot_offset = w_parser::vec2_from_str( iter_ad->find_value( "hotspot") );

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals();
				}
				else if( type == "anim_texture" )
				{
					auto asset_ptr = a_anim_texture::find( tag, b_silent( true ) );

					int frames_per_sec = w_parser::int_from_str( iter_ad->find_value( "frames_per_sec" ) );
					auto tween_type = static_cast<e_tween_type>( w_parser::int_from_str( iter_ad->find_value( "tween" ) ) );

					if( !asset_ptr )
					{
						asset_ptr = engine->asset_cache->add( std::make_unique<a_anim_texture>( tween_type, frames_per_sec ),
															  tag, "" );
					}

					// ------------------------------------------------------------------------

					const std::string_view frames = iter_ad->find_value( "frames");

					w_tokenizer tok( frames, ',' );
					while( !tok.is_eos() )
					{
						auto subtex = a_subtexture::find( *tok.get_next_token() );
						asset_ptr->add_frame( subtex );
					}

					// ------------------------------------------------------------------------

					asset_ptr->clean_up_internals();
					asset_ptr->create_internals();
				}
				else if( type == "subtexture" )
				{
					auto asset_ptr = a_subtexture::find( tag, b_silent( true ) );

					if( !asset_ptr )
					{
						w_rect rc = w_parser::rect_from_str( iter_ad->find_value( "rect") );
						asset_ptr = engine->asset_cache->add( std::make_unique<a_subtexture>( iter_ad->find_value( "texture" ), rc ),
															  tag, "" );
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
	for( const auto& line : *( file->lines.get() ) )
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
			//w_tokenizer tok_kv( line, '\"', b_parse_bracket_sets( false ) );
			w_tokenizer tok_kv( line, '\"' );

			//tok_kv.get_next_token();
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
