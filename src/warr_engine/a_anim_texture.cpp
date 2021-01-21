
#include "master_pch.h"
#include "master_header.h"

#if 0 // #anim_texture
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
	frame_tween = nullptr;
}

bool a_anim_texture::create_internals()
{
	float duration = ( 1000.f / static_cast<float>( frames_per_second ) ) * static_cast<float>( frames.size() );
	frame_tween = std::make_unique<w_tween>( tween_type, 0.0f, static_cast<float>( frames.size() ), duration );

	return true;
}

void a_anim_texture::add_frame( a_texture* frame )
{
	frames.emplace_back( frame );
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
	frame_tween->update();
}

a_texture* a_anim_texture::get_texture( float anim_offset )
{
	assert( !frames.empty() );	// did you forget to call "add_frame"?

	auto idx = (size_t)( frame_tween->get_ival( 0, static_cast<int>( frames.size() ) - 1 ) );

	if( !fequals( anim_offset, 0.0f ) )
	{
		idx += (size_t)( anim_offset * frames.size() );
	}

	idx = idx % frames.size();

	return frames[ idx ];
}
#endif
