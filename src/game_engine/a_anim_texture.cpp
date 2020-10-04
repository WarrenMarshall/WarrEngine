#include "master_pch.h"
#include "master_header.h"

a_anim_texture::a_anim_texture( e_tween_type tween_type, int frames_per_second )
	: tween_type( tween_type ), frames_per_second( frames_per_second )
{
}

a_anim_texture::~a_anim_texture()
{
	a_texture::~a_texture();
	a_anim_texture::clean_up_internals();
}

void a_anim_texture::clean_up_internals()
{
	frame_tween = nullptr;
}

bool a_anim_texture::create_internals()
{
	frame_tween = std::make_unique<w_tween>( tween_type, 0.0f, static_cast<float>( frames.size() ), static_cast<float>( frames_per_second ) );

	return true;
}

void a_anim_texture::add_frame( a_subtexture* subtex )
{
	frames.emplace_back( subtex );
}

/*
	moves the animation tween to a random value between start and end
*/
void a_anim_texture::randomize()
{
	assert( !frames.empty() );	// did you forget to call "add_frame"?

	frame_tween->randomize();
}

void a_anim_texture::update()
{
	assert( !frames.empty() );	// did you forget to call "add_frame"?

	frame_tween->update();
}

a_subtexture* a_anim_texture::get_subtexture( float anim_offset )
{
	assert( !frames.empty() );	// did you forget to call "add_frame"?

	int idx = frame_tween->get_ival();

	if( anim_offset )
	{
		idx += static_cast<int>( anim_offset * frames.size() );
	}

	idx = idx % frames.size();
	return frames[ idx ];
}

