
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

a_texture::~a_texture()
{
	clean_up_internals();
}

void a_texture::clean_up_internals()
{
	render_buffer = nullptr;

	if( id > 0 )
		glDeleteTextures( 1, &id );
}

bool a_texture::create_internals( bool is_hot_reloading )
{
	if( original_filename == "" )
		return true;
	
	auto file = engine->fs->load_file_into_memory( original_filename );
	int w, h, bpp;
	unsigned char* color_data = stbi_load_from_memory(
		(const stbi_uc*) ( file->buffer->data() ),
		static_cast<int>( file->buffer->size() ),
		&w, &h, &bpp, 4 );
	this->w = static_cast<float>( w );
	this->h = static_cast<float>( h );
	was_loaded_from_zip_file = file->was_loaded_from_zip_file;

	if( !color_data )
		log_error( "%s : couldn't load the file : [%s]", __FUNCTION__, original_filename.c_str() );

	// save the last time modified for hot reloading
	if( g_allow_hot_reload )
	{
		last_modified = get_last_modified_from_disk();
	}

	// create our render buffer
	render_buffer = std::make_unique<w_render_buffer>( GL_TRIANGLES );

	// upload texture to opengl

	glGenTextures( 1, &id );
	bind();

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, color_data );
	OPENGL->clear_texture_bind();

	stbi_image_free( color_data );
	color_data = nullptr;

	return true;
}

void a_texture::bind()
{
	glBindTexture( GL_TEXTURE_2D, id );
}

void a_texture::unbind()
{
	glBindTexture( GL_TEXTURE_2D, 0 );
}

a_subtexture* a_texture::get_subtexture( float offset )
{
	return subtex;
}

void a_texture::draw( e_render_pass render_pass )
{
	if( render_buffer )
	{
		bind();
		render_buffer->draw( static_cast<int>( render_pass ) );
		unbind();
	}
}

// ----------------------------------------------------------------------------

a_subtexture::a_subtexture( const std::string& tex_name )
{
	tex = engine->get_asset<a_texture>( tex_name );

	rc_src.w = sz.w = tex->w;
	rc_src.h = sz.h = tex->h;
}

a_subtexture::a_subtexture( const std::string& texture_name, const w_rect& rc )
{
	// find the texture being referenced
	tex = engine->get_asset<a_texture>( texture_name );

	rc_src = rc;

	if( rc.w == -1 ) { rc_src.w = tex->w; }
	if( rc.h == -1 ) { rc_src.h = tex->h; }

	rc_src.y = tex->h - rc.y - rc_src.h;

	sz.w = rc_src.w;
	sz.h = rc_src.h;

	uv00 = w_uv(
		rc_src.x / tex->w,
		rc_src.y / tex->h
	);
	uv11 = w_uv(
		( rc_src.x + rc_src.w ) / tex->w,
		( rc_src.y + rc_src.h ) / tex->h
	);
}

void a_subtexture::bind()
{
	tex->bind();
}

void a_subtexture::unbind()
{
	tex->unbind();
}

// ----------------------------------------------------------------------------

void a_gradient::clean_up_internals()
{
	render_buffer = nullptr;
}

bool a_gradient::create_internals( bool is_hot_reloading )
{
	int num_colors = static_cast<int>( colors.size() );
	w = ( alignment == e_align::horizontal ) ? num_colors : 1.0f;
	h = ( alignment == e_align::vertical ) ? num_colors : 1.0f;

	// create a buffer of color data, and fill it with the gradient colors

	float* color_data = (float*) ( ::malloc( sizeof( float ) * 4 * static_cast<int>( w ) * static_cast<int>( h ) ) );
	int clridx = 0;

	float* wptr = color_data;

	for( int y = 0; y < h; ++y )
	{
		for( int x = 0; x < w; ++x )
		{
			const w_color& clr = colors[clridx];

			*wptr++ = clr.r;
			*wptr++ = clr.g;
			*wptr++ = clr.b;
			*wptr++ = clr.a;

			clridx++;
		}
	}

	// create our render buffer
	render_buffer = std::make_unique<w_render_buffer>( GL_TRIANGLES );

	// upload the texture to opengl

	glGenTextures( 1, &id );
	bind();

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei)w, (GLsizei)h, 0, GL_RGBA, GL_FLOAT, color_data );
	OPENGL->clear_texture_bind();

	// clean up

	::free( color_data );

	return true;
}

// ----------------------------------------------------------------------------

a_anim_texture::a_anim_texture( e_tween_type tween_type, int frames_per_second )
	: tween_type( tween_type ), frames_per_second( frames_per_second )
{
	frame_tween = std::make_unique<w_tween>( tween_type, 0.0f, static_cast<float>( frames.size() - 1 ), static_cast<float>( frames_per_second ) );
}

void a_anim_texture::clean_up_internals()
{
	render_buffer = nullptr;
}

bool a_anim_texture::create_internals( bool is_hot_reloading )
{
	return true;
}

void a_anim_texture::add_frame( a_subtexture* tex )
{
	frames.emplace_back( tex );
}

/*
	moves the animation tween to a random value between start and end
*/
void a_anim_texture::randomize()
{
	assert( frames.size() );	// did you forget to call "add_frame"?

	frame_tween->randomize();
}

void a_anim_texture::update()
{
	assert( frames.size() );	// did you forget to call "add_frame"?

	frame_tween->update();
}

a_subtexture* a_anim_texture::get_subtexture( float offset )
{
	assert( frames.size() );	// did you forget to call "add_frame"?


	// apply random offset if needed

	int idx = frame_tween->get_ival();

	if( offset )
	{
		idx += static_cast<int>( offset * frames.size() );
	}

	idx = idx % frames.size();
	return frames[idx];
}

// ----------------------------------------------------------------------------

a_emitter_params::a_emitter_params()
{
	t_scale
		->kf_clear()
		->kf_add( w_keyframe( 0.00f, 1.0f ) )
		->kf_add( w_keyframe( 1.0f, 0.0f ) );

	t_alpha
		->kf_clear()
		->kf_add( w_keyframe( 0.00f, 0.0f ) )
		->kf_add( w_keyframe( 0.20f, 1.0f ) )
		->kf_add( w_keyframe( 1.0f, 0.0f ) );

	t_color
		->kf_clear()
		->kf_add( w_keyframe( 0.00f, W_COLOR_WHITE ) )
		->kf_add( w_keyframe( 1.0f, W_COLOR_WHITE ) );
}

bool a_emitter_params::create_internals( bool is_hot_reloading )
{
	if( g_allow_hot_reload )
	{
		if( is_hot_reloading )
		{
			send_event_to_listeners( e_event_id::emitter_params_hot_reload, this );
		}

		// save the last time modified for hot reloading
		last_modified = get_last_modified_from_disk();
	}

	return true;
}

// ----------------------------------------------------------------------------

bool a_font_def::create_internals( bool is_hot_reloading )
{
	ZeroMemory( char_map.data(), sizeof( w_font_char ) * max_font_chars );

	auto file = engine->fs->load_file_into_memory( original_filename );
	was_loaded_from_zip_file = file->was_loaded_from_zip_file;

	w_mem_file* mf = file.get();
	std::string file_as_string = std::string( file->buffer->begin(), file->buffer->end() );
	
	w_tokenizer tok( file_as_string, '\n', "" );

	float texw, texh, x, y, w, h;

	std::string line = tok.get_next_token();
	while( !tok.is_eos() )
	{
		if( line.substr( 0, 7 ) == "common " )
		{
			texw = w_parser::parse_float_value( w_parser::parse_key_value( line, "scaleW=" ) );
			texh = w_parser::parse_float_value( w_parser::parse_key_value( line, "scaleH=" ) );
		}
		else if( line.substr( 0, 5 ) == "char " )
		{
			// parse a char definition
			int char_id = w_parser::parse_int_value( w_parser::parse_key_value( line, "id=" ) );
			w_font_char* fch = &( char_map[ char_id ] );

			x = w_parser::parse_float_value( w_parser::parse_key_value( line, "x=" ) );
			y = w_parser::parse_float_value( w_parser::parse_key_value( line, "y=" ) );
			w = w_parser::parse_float_value( w_parser::parse_key_value( line, "width=" ) );
			h = w_parser::parse_float_value( w_parser::parse_key_value( line, "height=" ) );

			fch->xoffset = w_parser::parse_float_value( w_parser::parse_key_value( line, "xoffset=" ) );
			fch->yoffset = w_parser::parse_float_value( w_parser::parse_key_value( line, "yoffset=" ) );
			fch->xadvance = w_parser::parse_float_value( w_parser::parse_key_value( line, "xadvance=" ) );

			fch->w = w;
			fch->h = h;

			fch->img = std::make_unique<a_subtexture>( texture_name, w_rect( x, y, w, h ) );

			max_height = w_max( max_height, static_cast<int>( fch->h + fch->yoffset ) );
		}

		line = tok.get_next_token();
	}

	// save the last time modified for hot reloading
	if( g_allow_hot_reload )
		last_modified = get_last_modified_from_disk();

	return true;
}

// ----------------------------------------------------------------------------

/*
	computes how wide and how tall a string is using this font.
*/
w_vec2 a_font::get_string_extents( const std::string& text )
{
	w_font_char* pxch;
	w_vec2 bounds;

	bool inside_color_code = false;

	for( const auto& iter : text )
	{
		pxch = &( font_def->char_map[ iter ] );

		if( iter == '{' )
			inside_color_code = true;
		else if( iter == '}' )
			inside_color_code = false;
		else if( !inside_color_code )
		{
			bounds.x += pxch->xadvance;
			bounds.y = w_max( bounds.y, pxch->h + pxch->yoffset );
		}
	}

	return bounds;
}

// ----------------------------------------------------------------------------

bool a_9slice_def::create_internals( bool is_hot_reloading )
{
	// save the last time modified for hot reloading
	if( g_allow_hot_reload )
		last_modified = get_last_modified_from_disk();

	return true;
}

// ----------------------------------------------------------------------------

a_sound::~a_sound()
{
	clean_up_internals();
}

void a_sound::clean_up_internals()
{
	if( snd > -1 )
	{
		BASS_SampleFree( snd );
		snd = -1;
	}
}

void a_sound::play()
{
	BASS_ChannelPlay( channel, true );
}

bool a_sound::create_internals( bool is_hot_reloading )
{
	auto file = engine->fs->load_file_into_memory( original_filename );
	snd = BASS_SampleLoad( true, file->buffer->data(), 0, static_cast<int>( file->buffer->size() ), 1, 0 );
	was_loaded_from_zip_file = file->was_loaded_from_zip_file;

	bool file_exists = engine->fs->file_exists_on_disk_or_in_zip( original_filename );

	// if BASS_SampleLoad failed to load the file BUT it does actually exist, it may simply be
	// that the user doesn't have an audio device connected to their computer. This is not
	// fatal to the engine and should not crash you out.

	if( !snd && !file_exists )
		log_error( "%s : couldn't load the file : [%s]", __FUNCTION__, name.c_str() );

	// save the last time modified for hot reloading
	if( g_allow_hot_reload )
		last_modified = get_last_modified_from_disk();

	channel = BASS_SampleGetChannel( snd, false );

	return true;
}

// ----------------------------------------------------------------------------

a_music::~a_music()
{
	clean_up_internals();
}

void a_music::play()
{
	stop();
	BASS_ChannelPlay( channel, false );
}

void a_music::stop()
{
	if( channel > -1 )
	{
		BASS_ChannelStop( channel );
		channel = -1;
	}
}

void a_music::clean_up_internals()
{
	if( mus > -1 )
		BASS_SampleFree( mus );
}

bool a_music::create_internals( bool is_hot_reloading )
{
	auto file = engine->fs->load_file_into_memory( original_filename );
	mus = BASS_SampleLoad( true, file->buffer->data(), 0, static_cast<int>( file->buffer->size() ), 1, BASS_SAMPLE_LOOP );
	was_loaded_from_zip_file = file->was_loaded_from_zip_file;

	bool file_exists = engine->fs->file_exists_on_disk_or_in_zip( original_filename );

	// if BASS_SampleLoad failed to load the file BUT it does actually exist, it may simply be
	// that the user doesn't have an audio device connected to their computer. This is not
	// fatal to the engine and should not crash you out.

	if( !mus && !file_exists )
		log_error( "%s : couldn't load the file : [%s]", __FUNCTION__, name.c_str() );

	// save the last time modified for hot reloading
	if( g_allow_hot_reload && is_hot_reloading )
		last_modified = get_last_modified_from_disk();

	channel = BASS_SampleGetChannel( mus, false );

	return true;
}
