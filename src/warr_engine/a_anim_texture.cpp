
#include "master_pch.h"
#include "master_header.h"

implement_find_func( a_anim_texture )

a_anim_texture::a_anim_texture( e_tween_type tween_type, int frames_per_second )
	: tween_type( tween_type ), frames_per_second( frames_per_second )
{
}

a_anim_texture::~a_anim_texture()
{
	a_anim_texture::clean_up_internals();
}

void a_anim_texture::clean_up_internals()
{
}

bool a_anim_texture::create_internals()
{
	auto duration_ms = ( 1000 / frames_per_second ) * frames.size();
	frame_tween = w_tween( 0.0f, static_cast<float>( frames.size() ), duration_ms, tween_type, tween_via::linear );

	return true;
}

void a_anim_texture::add_frame( const a_texture* frame )
{
	frames.emplace_back( frame );
}

// moves the animation tween to a random value between start and end

void a_anim_texture::randomize()
{
	assert( !frames.empty() );	// did you forget to call "add_frame"?

	frame_tween.randomize();
}

const a_texture* a_anim_texture::get_texture( float anim_offset )
 {
	assert( !frames.empty() );	// did you forget to call "add_frame"?

	auto idx = (int)( *frame_tween );

	if( !fequals( anim_offset, 0.0f ) )
	{
		idx += (size_t)( anim_offset * frames.size() );
	}

	idx = idx % frames.size();

	return frames[ idx ];
 }
