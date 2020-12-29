
#include "app_header.h"

// ----------------------------------------------------------------------------

void e_paddle::phys_begin_contact( w_pending_collision& coll, w_entity* other )
{
	if( other->tag == "ball" )
	{
		// stop ball
		other->phys_get_primary_body()->body->SetLinearVelocity( w_vec2( 0, 0 ).as_b2Vec2() );

		// compute direction away from paddles center

		w_vec2 push_away_dir = ( other->pos - pos ).normalize();
		push_away_dir *= e_ball::speed;

		// push ball in that new direction
		other->phys_get_primary_body()->body->ApplyForceToCenter( push_away_dir.to_b2d().as_b2Vec2(), true );
	}
}

// ----------------------------------------------------------------------------

e_ball::e_ball()
{
	set_tag( "ball" );
	set_collision( clayer_ball, clayer_paddle | clayer_ball | clayer_world_left_right | clayer_world_top_bottom );

	auto ecd = add_component<ec_b2d_dynamic>();
	ecd->is_primary_body = true;
	ecd->add_fixture_circle( "", w_vec2::zero, e_ball::radius );
	//ecd->body->SetFixedRotation( true );

	add_component<ec_primitive_shape>()->init( primitive_shape::filled_rectangle, w_color::pal( 4 ) * 1.1f, w_rect( -e_ball::radius, -e_ball::radius, e_ball::radius * 2.f, e_ball::radius * 2.f ) );
	//add_component<ec_sprite>()->init( "sub_ball_body" );
}

void e_ball::phys_begin_contact( w_pending_collision& coll, w_entity* other )
{
	if( other->tag == "ball" )
	{
		// stop balls
		phys_get_primary_body()->body->SetLinearVelocity( w_vec2( 0, 0 ).as_b2Vec2() );
		other->phys_get_primary_body()->body->SetLinearVelocity( w_vec2( 0, 0 ).as_b2Vec2() );

		// compute new directions
		b2Vec2 world_point = other->phys_get_primary_body()->body->GetWorldPoint( coll.manifold.localPoint );

		w_vec2 world_point_a = w_vec2( coll.fixture_a->GetBody()->GetWorldPoint( coll.manifold.localPoint ) ).from_b2d();
		w_vec2 world_point_b = w_vec2( coll.fixture_b->GetBody()->GetWorldPoint( coll.manifold.localPoint ) ).from_b2d();

		w_vec2 mid_point = world_point_a + ( ( world_point_b - world_point_a ) * 0.5f );

		// push balls in new directions
		phys_get_primary_body()->body->ApplyForceToCenter( ( ( pos - mid_point ).normalize() * e_ball::speed ).to_b2d().as_b2Vec2(), true );
		other->phys_get_primary_body()->body->ApplyForceToCenter( ( ( other->pos - mid_point ) * e_ball::speed ).normalize().to_b2d().as_b2Vec2(), true );
	}

	if( other->tag == "world" )
	{
		ilc_set( life_cycle::dying );
		static_cast<layer_game*>( LAYER )->spawn_ball();
	}
}
