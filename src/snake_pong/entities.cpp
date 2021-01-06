
#include "app_header.h"

// ----------------------------------------------------------------------------

void e_paddle::on_collision_begin( w_pending_collision& coll, w_entity* other )
{
	// balls bounce off of paddles

	if( other->tag == "ball" )
	{
		// stop ball
		other->get_component<ec_physics>()->get_primary_body()->body->SetLinearVelocity( w_vec2( 0, 0 ).as_b2Vec2() );

		// compute direction away from paddles center

		w_vec2 push_away_dir = ( other->get_tform()->pos - get_tform()->pos ).normalize();
		push_away_dir *= e_ball::speed;

		// push ball in that new direction
		other->get_component<ec_physics>()->get_primary_body()->body->ApplyForceToCenter( push_away_dir.to_b2d().as_b2Vec2(), true );
	}
}

// ----------------------------------------------------------------------------

e_ball::e_ball()
{
	set_tag( "ball" );
	get_component<ec_physics>()->set_collision_flags( clayer_ball, clayer_paddle | clayer_ball | clayer_world_left_right | clayer_world_top_bottom );

	auto ecd = add_component<ec_b2d_dynamic>();
	ecd->is_primary_body = true;
	ecd->add_fixture_circle( "", w_vec2::zero, e_ball::radius );
	ecd->body->SetFixedRotation( true );

	add_component<ec_sprite>()->init( "ball_body" );
	add_component<ec_sprite>()->init( "ball_lighting" );
}

void e_ball::on_collision_begin( w_pending_collision& coll, w_entity* other )
{
	// balls bounce off of each other

	if( other->tag == "ball" )
	{
		// stop balls
		get_component<ec_physics>()->get_primary_body()->body->SetLinearVelocity( w_vec2( 0, 0 ).as_b2Vec2() );
		other->get_component<ec_physics>()->get_primary_body()->body->SetLinearVelocity( w_vec2( 0, 0 ).as_b2Vec2() );

		// compute new directions
		b2Vec2 world_point = other->get_component<ec_physics>()->get_primary_body()->body->GetWorldPoint( coll.manifold.localPoint );

		w_vec2 world_point_a = w_vec2( coll.fixture_a->GetBody()->GetWorldPoint( coll.manifold.localPoint ) ).from_b2d();
		w_vec2 world_point_b = w_vec2( coll.fixture_b->GetBody()->GetWorldPoint( coll.manifold.localPoint ) ).from_b2d();

		w_vec2 mid_point = world_point_a + ( ( world_point_b - world_point_a ) * 0.5f );

		// push balls in new directions
		get_component<ec_physics>()->get_primary_body()->body->ApplyForceToCenter( ( ( get_tform()->pos - mid_point ).normalize() * e_ball::speed ).to_b2d().as_b2Vec2(), true );
		other->get_component<ec_physics>()->get_primary_body()->body->ApplyForceToCenter( ( ( other->get_tform()->pos - mid_point ) * e_ball::speed ).normalize().to_b2d().as_b2Vec2(), true );
	}

	// touching the world kills the ball

	if( other->tag == "world" )
	{
		ilc_set( life_cycle::dying );
		static_cast<layer_game*>( LAYER )->spawn_ball();
	}
}
