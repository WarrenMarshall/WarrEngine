
#include "app_header.h"

layer_pong::layer_pong()
{
	draws_completely_solid = true;
}

void layer_pong::push()
{
	pong_physics = std::make_unique<w_pong_physics>();
	engine->box2d_world->SetContactListener( pong_physics.get() );

	engine->box2d_world->SetGravity( { 0, 0 } );
	ec_b2d_body* ec = nullptr;

	// world geo

	world_geo = add_entity<w_entity>();
	world_geo->collision_layer = clayer_world;
	world_geo->collides_with = clayer_ball;
	world_geo->draw_debug_info = true;

	ec = world_geo->add_component<ec_b2d_static>();
	{
		// bounding box for world

		ec->add_fixture_line_loop(
			sensor_id::world,
			w_vec2::zero,
			{
				{ 4.0f, 4.0f },
				{ v_window_w - 8.0f, 4.0f },
				{ v_window_w - 8.0f, v_window_h - 8.0f },
				{ 4.0f, v_window_h - 8.0f }
			}
		);
	}

	{
		ball = add_entity<w_entity>();
		ball->draw_debug_info = true;
		ball->collision_layer = clayer_ball;
		ball->collides_with = clayer_world;

		ball->set_transform( { v_window_hw, v_window_hh }, 0, 1 );

		auto ecd = ball->add_component<ec_b2d_dynamic>();
		auto f = ecd->add_fixture_circle( sensor_id::ball, w_vec2::zero, 6 );
		//f->SetRestitution( 1.0f );
		//f->SetFriction( 0.0f );

		ecd->body->SetLinearVelocity( w_vec2::get_random_unit() * 2.f );
		//ecd->body->SetLinearDamping( 0.0f );
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
