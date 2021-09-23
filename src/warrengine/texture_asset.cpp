
#include "master_pch.h"
#include "master_header.h"

namespace war
{

war::Texture_Source_Base* Texture_Asset::get_src_texture() const
{
	// this shouldn't be called for animated textures, only their frames
	//
	// animated textures are represented by a_textures in the cache but
	// they are really just holders for animation data. as such, they don't
	// have things like src_textures or UV coordinates associated with them
	// directy. only their frames have that.

	assert( !is_animated() );

	return src_texture;
}

// single frame, uses entire source texture

Texture_Asset::Texture_Asset( std::string_view src_texture_tag )
{
	src_texture = find_texture_source_base( src_texture_tag );

	rc = { 0.f, 0.f, src_texture->w, src_texture->h };

#ifndef _FINAL_RELEASE
	if( ( rc.x + rc.w ) > src_texture->w or ( rc.y + rc.h ) > src_texture->h )
	{
		log_fatal( "texture extends outside the bounds of the src_texture : {}", std::string( src_texture_tag ) );
	}
#endif
}

Texture_Asset::Texture_Asset( std::string_view src_texture_tag, const Rect& rc_tex )
{
	src_texture = find_texture_source_base( src_texture_tag );

	rc = rc_tex;

	// images are upside down, so the Y coordinate gets flipped across the V axis

	Rect rc_src(
		rc_tex.x,
		src_texture->h - rc_tex.y - rc_tex.h,
		rc_tex.w,
		rc_tex.h
	);

	// texture extends outside the bounds of the src_texture
	assert( ( rc.x + rc.w ) <= src_texture->w and ( rc.y + rc.h ) <= src_texture->h );

	// compute uv coordinates for the top left and bottom right corners. these corners
	// provide enough information to reconstruct UV01 and UV10 when needed.

	uv00 = Vec2(
		rc_src.x / src_texture->w,
		rc_src.y / src_texture->h
	);
	uv11 = Vec2(
		( rc_src.x + rc_src.w ) / src_texture->w,
		( rc_src.y + rc_src.h ) / src_texture->h
	);
}

// animated, uses a series of textures on a timed loop

Texture_Asset::Texture_Asset( std::vector<Texture_Asset*>& frames, e_tween_type tween_type, int32_t frames_per_second )
	: frames( frames ), tween_type( tween_type ), frames_per_second( frames_per_second )
{
	auto duration_ms = ( 1000 / frames_per_second ) * frames.size();
	frame_tween = Tween( 0.f, (float_t)( frames.size() ), duration_ms, tween_type, e_tween_via::linear );
}

Texture_Source_Base* Texture_Asset::find_texture_source_base( std::string_view src_texture_tag )
{
	Texture_Source_Base* result = g_engine->find_asset_safe<Texture_Source_Asset>( src_texture_tag );

	if( !result )
	{
		result = g_engine->find_asset_safe<Gradient_Source_Asset>( src_texture_tag );
	}
	assert( result );

	return result;
}

const Texture_Asset* Texture_Asset::get_frame( float_t anim_offset ) const
{
	if( !is_animated() )
	{
		return this;
	}

	// use the tween to get the animation frame index, clamped to the value range
	auto idx = (size_t)( glm::clamp<float_t>( const_cast<Texture_Asset*>( this )->frame_tween.value(), 0.f, (float_t)frames.size() ) );
	idx += (size_t)( anim_offset * frames.size() );
	idx = idx % frames.size();

	return frames[ idx ];
}

// moves the animation tween to a random value between start and end

void Texture_Asset::randomize()
{
	frame_tween.randomize();
}

bool Texture_Asset::is_animated() const
{
	return frames_per_second;
}

float_t Texture_Asset::width() const
{
	return get_frame( 0.f )->rc.w;
}

float_t Texture_Asset::height() const
{
	return get_frame( 0.f )->rc.h;
}

}
