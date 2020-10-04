#include "master_pch.h"
#include "master_header.h"

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
		->kf_add( w_keyframe( 0.00f, w_color::white ) )
		->kf_add( w_keyframe( 1.0f, w_color::white ) );
}

