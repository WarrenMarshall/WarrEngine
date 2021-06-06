#include "master_pch.h"
#include "master_header.h"

namespace war
{

emitter_parameters_asset::emitter_parameters_asset()
{
	needs_warm_up = false;
	is_one_shot = false;

	t_scale
		.clear_key_frames()
		->add_key_frame( timeline_key_frame( 0.f, 1.f ) )
		->add_key_frame( timeline_key_frame( 1.f, 0.f ) );

	t_alpha
		.clear_key_frames()
		->add_key_frame( timeline_key_frame( 0.f, 0.f ) )
		->add_key_frame( timeline_key_frame( 1.f, 0.f ) );

	t_color
		.clear_key_frames()
		->add_key_frame( timeline_key_frame( 0.f, color::white ) )
		->add_key_frame( timeline_key_frame( 1.f, color::black ) );
}

}
