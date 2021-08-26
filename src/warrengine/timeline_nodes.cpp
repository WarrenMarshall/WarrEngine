
#include "master_pch.h"
#include "master_header.h"

namespace war
{

timeline_nodes::timeline_nodes()
{
	life_cycle.set( life_cycle::dead );
}

void timeline_nodes::clear( time_ms duration )
{
	key_frames.clear();
	this->duration = duration;
}

void timeline_nodes::go()
{
	start = g_engine->time.now();
	end = start + duration;

	life_cycle.set( life_cycle::alive );

	for( auto& kf : key_frames )
	{
		kf.life_cycle.set( life_cycle::alive );
		kf.is_running = false;
	}
}

void timeline_nodes::update()
{
	if( key_frames.empty() )
	{
		return;
	}

	if( !life_cycle.is_alive() )
	{
		return;
	}

	float pct_on_timeline = glm::clamp( ( g_engine->time.now() - start ) / (float)duration, 0.f, 1.f );

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
		life_cycle.set( life_cycle::dead );
	}
}

void timeline_nodes::add_kf_shake_angle( bool should_restore_state, float pct_marker, time_ms duration, transform* tform, float strength )
{
	key_frames.emplace_back( tnkf_type::shake_angle, should_restore_state, pct_marker, duration );
	auto kf = &key_frames.back();

	kf->shake_angle.tform = tform;
	kf->shake_angle.strength = strength;
}

void timeline_nodes::add_kf_pp_color_overlay( bool should_restore_state, float pct_marker, time_ms duration, color color_overlay )
{
	key_frames.emplace_back( tnkf_type::pp_color_overlay, should_restore_state, pct_marker, duration );
	auto kf = &key_frames.back();

	kf->pp_color_overlay.clr = color_overlay;
}

void timeline_nodes::add_kf_play_sound( bool should_restore_state, float pct_marker, sound_asset* snd )
{
	key_frames.emplace_back( tnkf_type::play_sound, should_restore_state, pct_marker, 0 );
	auto kf = &key_frames.back();

	kf->play_sound.snd = snd;
}

void timeline_nodes::add_kf_scene_push_under( bool should_restore_state, float pct_marker, std::unique_ptr<scene> scene_to_push )
{
	key_frames.emplace_back( tnkf_type::scene_push_under, should_restore_state, pct_marker, 0 );
	auto kf = &key_frames.back();

	kf->scene_push_under.new_scene = scene_to_push.get();
	scene_to_push.release();
}

void timeline_nodes::add_kf_scene_pop_at_offset( bool should_restore_state, float pct_marker, int offset )
{
	key_frames.emplace_back( tnkf_type::scene_pop_at_offset, should_restore_state, pct_marker, 0 );
	auto kf = &key_frames.back();

	kf->scene_pop_at_offset.offset = offset;
}

}
