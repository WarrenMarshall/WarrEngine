
#include "master_pch.h"
#include "master_header.h"

namespace war
{

Timeline_Nodes::Timeline_Nodes()
{
	life_cycle.set( e_life_cycle::dead );
}

void Timeline_Nodes::clear( time_ms duration )
{
	key_frames.clear();

	// add a tiny bit of time to the duration to make sure any key_frames that
	// are the same length as the timeline itself get a chance to finish up.
	duration += 50;

	this->duration = duration;
}

void Timeline_Nodes::go()
{
	start = g_engine->clock.now();
	end = start + duration;

	life_cycle.set( e_life_cycle::alive );

	for( auto& kf : key_frames )
	{
		kf.life_cycle.set( e_life_cycle::alive );
		kf.is_running = false;
	}
}

void Timeline_Nodes::update()
{
	if( key_frames.empty() )
	{
		return;
	}

	if( !life_cycle.is_alive() )
	{
		return;
	}

	float pct_on_timeline = glm::clamp( ( g_engine->clock.now() - start ) / (float)duration, 0.f, 1.f );

	// for every node_key_frame that is behind the current pct marker of this
	// timeline, call it's update() function.

	for( auto& kf : key_frames )
	{
		if( kf.life_cycle.is_alive() and kf.pct_marker <= pct_on_timeline )
		{
			kf.update();
		}
	}

	if( fequals( pct_on_timeline, 1.f ) )
	{
		life_cycle.set( e_life_cycle::dead );
	}
}

void Timeline_Nodes::add_kf_shake_angle( bool should_restore_state, float pct_marker, time_ms duration, Transform* tform, float strength )
{
	key_frames.emplace_back( e_tnkf_type::shake_angle, should_restore_state, pct_marker, duration );
	auto kf = &key_frames.back();

	kf->shake_angle.tform = tform;
	kf->shake_angle.strength = strength;
}

void Timeline_Nodes::add_kf_pp_color_overlay( bool should_restore_state, float pct_marker, time_ms duration, Color color )
{
	key_frames.emplace_back( e_tnkf_type::pp_color_overlay, should_restore_state, pct_marker, duration );
	auto kf = &key_frames.back();

	kf->pp_color_overlay.color = color;
}

void Timeline_Nodes::add_kf_pp_pixelate( bool should_restore_state, float pct_marker, time_ms duration, float pixelate_factor )
{
	key_frames.emplace_back( e_tnkf_type::pp_pixelate, should_restore_state, pct_marker, duration );
	auto kf = &key_frames.back();

	kf->pp_pixelate.pixelate_factor = pixelate_factor;
}

void Timeline_Nodes::add_kf_play_sound( bool should_restore_state, float pct_marker, Sound_Asset* snd )
{
	key_frames.emplace_back( e_tnkf_type::play_sound, should_restore_state, pct_marker, 0 );
	auto kf = &key_frames.back();

	kf->play_sound.snd = snd;
}

void Timeline_Nodes::add_kf_scene_push_under( bool should_restore_state, float pct_marker, std::unique_ptr<scene> scene_to_push )
{
	key_frames.emplace_back( e_tnkf_type::scene_push_under, should_restore_state, pct_marker, 0 );
	auto kf = &key_frames.back();

	kf->scene_push_under.new_scene = scene_to_push.get();
	scene_to_push.release();
}

void Timeline_Nodes::add_kf_scene_pop_under( bool should_restore_state, float pct_marker )
{
	key_frames.emplace_back( e_tnkf_type::scene_pop_under, should_restore_state, pct_marker, 0 );
}

void Timeline_Nodes::add_kf_reset_scene_stack_to_main_menu( bool should_restore_state, float pct_marker )
{
	key_frames.emplace_back( e_tnkf_type::reset_scene_stack_to_main_menu, should_restore_state, pct_marker, 0 );
}

}
