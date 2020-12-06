
#include "app_header.h"

// ----------------------------------------------------------------------------

void e_paddle::phys_begin_contact( const w_physics_responder& responder, const char* other_tag, w_entity* other )
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

void e_ball::phys_begin_contact( const w_physics_responder& responder, const char* other_tag, w_entity* other )
{
	if( other->tag == "ball" )
	{
		// stop balls
		phys_get_primary_body()->body->SetLinearVelocity( w_vec2( 0, 0 ).as_b2Vec2() );
		other->phys_get_primary_body()->body->SetLinearVelocity( w_vec2( 0, 0 ).as_b2Vec2() );

		// compute new directions
		b2Vec2 world_point = other->phys_get_primary_body()->body->GetWorldPoint( responder.manifold->localPoint );

		w_vec2 world_point_a = w_vec2( responder.contact->GetFixtureA()->GetBody()->GetWorldPoint( responder.manifold->localPoint ) ).from_b2d();
		w_vec2 world_point_b = w_vec2( responder.contact->GetFixtureB()->GetBody()->GetWorldPoint( responder.manifold->localPoint ) ).from_b2d();

		w_vec2 mid_point = world_point_a + ( ( world_point_b - world_point_a ) * 0.5f );

		// push balls in new directions
		phys_get_primary_body()->body->ApplyForceToCenter( ( ( pos - mid_point ).normalize() * e_ball::speed ).to_b2d().as_b2Vec2(), true );
		other->phys_get_primary_body()->body->ApplyForceToCenter( ( ( other->pos - mid_point ) * e_ball::speed ).normalize().to_b2d().as_b2Vec2(), true );
	}
}
