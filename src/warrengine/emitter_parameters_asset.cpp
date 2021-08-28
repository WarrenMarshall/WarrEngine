#include "master_pch.h"
#include "master_header.h"

namespace war
{

Emitter_Parameters_Asset::Emitter_Parameters_Asset()
{
	needs_warm_up = false;
	is_one_shot = false;

	t_scale
		.clear_key_frames()
		->add_key_frame( Timeline_Values_Key_Frame( 0.f, 1.f ) )
		->add_key_frame( Timeline_Values_Key_Frame( 1.f, 0.f ) );

	t_alpha
		.clear_key_frames()
		->add_key_frame( Timeline_Values_Key_Frame( 0.f, 0.f ) )
		->add_key_frame( Timeline_Values_Key_Frame( 1.f, 0.f ) );

	t_color
		.clear_key_frames()
		->add_key_frame( Timeline_Values_Key_Frame( 0.f, Color::white ) )
		->add_key_frame( Timeline_Values_Key_Frame( 1.f, Color::black ) );
}

}
