
#include "master_pch.h"
#include "master_header.h"

namespace war
{

Timeline_Of_Nodes::Key_Frame::Key_Frame( e_tnkf_type type, bool should_restore_state, float_t pct_marker, time_ms duration )
	: type( type ), should_restore_state( should_restore_state ), pct_marker( pct_marker ), duration( duration )
{
}

void Timeline_Of_Nodes::Key_Frame::on_started_running()
{
	started = g_engine->clock.now();

	switch( type )
	{
		// ----------------------------------------------------------------------------
		case e_tnkf_type::shake_angle:
		{
			shake_angle.save.angle = shake_angle.tform->angle;
		}
		break;

		// ----------------------------------------------------------------------------
		case e_tnkf_type::pp_color_overlay:
		{
			pp_color_overlay.save.color = g_engine->opengl_mgr.get_uniform_color( "u_color_overlay" );
		}
		break;

		// ----------------------------------------------------------------------------
		case e_tnkf_type::pp_pixelate:
		{
			pp_pixelate.save.pixelate_factor = g_engine->opengl_mgr.get_uniform_float( "u_pixelate_factor" );
		}
		break;

		// ----------------------------------------------------------------------------
		case e_tnkf_type::play_sound:
		{
			play_sound.snd->play();
			life_cycle.set( e_life_cycle::dead );
		}
		break;

		// ----------------------------------------------------------------------------
		case e_tnkf_type::scene_push_under:
		{
			g_engine->scene_mgr.push_under( scene_push_under.new_scene );
			life_cycle.set( e_life_cycle::dead );
		}
		break;

		// ----------------------------------------------------------------------------
		case e_tnkf_type::scene_pop_under:
		{
			g_engine->scene_mgr.pop_under();
			life_cycle.set( e_life_cycle::dead );
		}
		break;

		// ----------------------------------------------------------------------------
		case e_tnkf_type::reset_scene_stack_to_main_menu:
		{
			g_base_game->reset_scene_stack_to_main_menu();
			life_cycle.set( e_life_cycle::dead );
		}
		break;
	}
}

void Timeline_Of_Nodes::Key_Frame::update()
{
	if( !is_running )
	{
		is_running = true;
		on_started_running();
	}

	auto time_now = g_engine->clock.now();
	auto pct = glm::clamp( ( time_now - started ) / (float_t)duration, 0.f, 1.f );

	switch( type )
	{
		// ----------------------------------------------------------------------------
		case e_tnkf_type::shake_angle:
		{
			shake_angle.tform->angle = shake_angle.save.angle + ( shake_angle.noise.get() * shake_angle.strength );

			if( pct == 1.f )
			{
				if( should_restore_state )
				{
					shake_angle.tform->angle = shake_angle.save.angle;
				}
				life_cycle.set( e_life_cycle::dead );
			}
		}
		break;

		// ----------------------------------------------------------------------------
		case e_tnkf_type::pp_color_overlay:
		{
			g_engine->opengl_mgr.set_uniform_color( "u_color_overlay",
				make_color( pp_color_overlay.color, lerp( pp_color_overlay.save.color.a, pp_color_overlay.color.a, pct ) )
			);

			if( pct == 1.f )
			{
				if( should_restore_state )
				{
					g_engine->opengl_mgr.set_uniform_color( "u_color_overlay", pp_color_overlay.save.color );
				}
				life_cycle.set( e_life_cycle::dead );
			}
		}
		break;

		// ----------------------------------------------------------------------------
		case e_tnkf_type::pp_pixelate:
		{
			g_engine->opengl_mgr.set_uniform_float( "u_pixelate_factor",
				lerp( pp_pixelate.save.pixelate_factor, pp_pixelate.pixelate_factor, pct )
			);

			if( pct == 1.f )
			{
				if( should_restore_state )
				{
					g_engine->opengl_mgr.set_uniform_float( "u_pixelate_factor", pp_pixelate.save.pixelate_factor );
				}
				life_cycle.set( e_life_cycle::dead );
			}
		}
		break;
	}
}

// ----------------------------------------------------------------------------

Timeline_Of_Nodes::Timeline_Of_Nodes()
{
	life_cycle.set( e_life_cycle::dead );
}

void Timeline_Of_Nodes::init( time_ms duration )
{
	key_frames.clear();

	// add a tiny bit of time to the duration to make sure any key_frames that
	// are the same length as the timeline itself get a chance to finish up.
	duration += 50;

	this->duration = duration;
}

void Timeline_Of_Nodes::restart()
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

void Timeline_Of_Nodes::update()
{
	if( key_frames.empty() )
	{
		return;
	}

	if( !life_cycle.is_alive() )
	{
		return;
	}

	float_t pct_on_timeline = glm::clamp( ( g_engine->clock.now() - start ) / (float_t)duration, 0.f, 1.f );

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

void Timeline_Of_Nodes::add_kf_shake_angle( bool should_restore_state, float_t pct_marker, time_ms duration, Transform* tform, float_t strength )
{
	key_frames.emplace_back( e_tnkf_type::shake_angle, should_restore_state, pct_marker, duration );
	auto kf = &key_frames.back();

	kf->shake_angle.tform = tform;
	kf->shake_angle.strength = strength;
}

void Timeline_Of_Nodes::add_kf_pp_color_overlay( bool should_restore_state, float_t pct_marker, time_ms duration, Color color )
{
	key_frames.emplace_back( e_tnkf_type::pp_color_overlay, should_restore_state, pct_marker, duration );
	auto kf = &key_frames.back();

	kf->pp_color_overlay.color = color;
}

void Timeline_Of_Nodes::add_kf_pp_pixelate( bool should_restore_state, float_t pct_marker, time_ms duration, float_t pixelate_factor )
{
	key_frames.emplace_back( e_tnkf_type::pp_pixelate, should_restore_state, pct_marker, duration );
	auto kf = &key_frames.back();

	kf->pp_pixelate.pixelate_factor = pixelate_factor;
}

void Timeline_Of_Nodes::add_kf_play_sound( bool should_restore_state, float_t pct_marker, const Sound_Asset* snd )
{
	key_frames.emplace_back( e_tnkf_type::play_sound, should_restore_state, pct_marker, 0 );
	auto kf = &key_frames.back();

	kf->play_sound.snd = const_cast<Sound_Asset*>( snd );
}

void Timeline_Of_Nodes::add_kf_scene_push_under( bool should_restore_state, float_t pct_marker, std::unique_ptr<Scene> scene_to_push )
{
	key_frames.emplace_back( e_tnkf_type::scene_push_under, should_restore_state, pct_marker, 0 );
	auto kf = &key_frames.back();

	kf->scene_push_under.new_scene = scene_to_push.get();
	scene_to_push.release();
}

void Timeline_Of_Nodes::add_kf_scene_pop_under( bool should_restore_state, float_t pct_marker )
{
	key_frames.emplace_back( e_tnkf_type::scene_pop_under, should_restore_state, pct_marker, 0 );
}

void Timeline_Of_Nodes::add_kf_reset_scene_stack_to_main_menu( bool should_restore_state, float_t pct_marker )
{
	key_frames.emplace_back( e_tnkf_type::reset_scene_stack_to_main_menu, should_restore_state, pct_marker, 0 );
}

}
