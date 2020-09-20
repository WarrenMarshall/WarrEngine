
#include "app_header.h"

constexpr e_collision_layer layer_geo = collision_layer::layer1;
constexpr e_collision_layer layer_ball = collision_layer::layer2;
constexpr e_collision_layer layer_paddle = collision_layer::layer3;

// ----------------------------------------------------------------------------

layer_pong::layer_pong()
{
	draws_completely_solid = true;
}

void layer_pong::push()
{
	engine->box2d_world->SetContactListener( &contact_listener );

	engine->box2d_world->SetGravity( { 0, 0 } );

	world_geo = add_entity<w_entity>();
	world_geo->collision_layer = layer_geo;
	world_geo->draw_debug_info = true;

	float sz = 16;
	auto ec = world_geo->add_component<ec_b2d_static>()->init_as_chain(
		{
			{ sz, sz },
			{ sz, v_window_h - sz },
			{ v_window_w - sz, v_window_h - sz },
			{ v_window_w - sz, sz }
		}
	);

	ec->body->GetFixtureList()->SetRestitution( 1.0f );
	ec->body->GetFixtureList()->SetFriction( 0.0f );
	ec->body->SetLinearDamping( 0.0f );

#if 0
	for( int x = 0 ; x < 5 ; ++x )
	{
		float xpos = w_random::getf_range( 0.0f, v_window_w );
		float ypos = w_random::getf_range( 64.0f, v_window_h );

		if( w_random::geti_range( 0, 4 ) == 45 )
		{
			float sz = w_random::getf_range( 4, 16 );
			world_geo->add_component<ec_b2d_static>()->init_as_circle( { xpos, ypos }, sz );
		}
		else
		{
			float sz = w_random::getf_range( 4, 32 );
			float sz2 = w_random::getf_range( 4, 32 );
			world_geo->add_component<ec_b2d_static>()->init_as_box( { xpos, ypos }, sz, sz2 );
		}
	}
#endif

	{
		ball = add_entity<w_entity>();
		ball->draw_debug_info = true;
		ball->collision_layer = layer_ball;
		ball->set_transform( { v_window_hw, v_window_hh }, 0, 1 );
		auto ecd = ball->add_component<ec_b2d_dynamic>()->init_as_circle( 6 );
		ecd->body->GetFixtureList()->SetRestitution( 1.0f );
		ecd->body->GetFixtureList()->SetFriction( 0.0f );

		ecd->body->SetLinearVelocity( w_vec2::get_random_unit() * 2.f );
		ecd->body->SetLinearDamping( 0.0f );
	}
}

void layer_pong::update()
{
	w_layer::update();

	//auto ec = ball->get_component<ec_b2d_dynamic>();
	//b2Vec2 vel = ec->body->GetLinearVelocity(
	//ec->body->SetLinearVelocity( w_vec2::get_random_unit() * 2.f );
}

void layer_pong::draw()
{
	w_layer::draw();

}
