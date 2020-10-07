
#include "master_pch.h"
#include "master_header.h"

/// Draw a closed polygon provided in CCW order.
void w_physics_debug_draw::DrawPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color )
{
	RENDER
		->begin()
		->push_rgba( { color.r, color.g, color.b, color.a } );

	for( int v = 0 ; v < vertexCount ; ++v )
	{
		w_vec2 v0 = w_vec2( vertices[ v ] ).from_b2d();
		w_vec2 v1 = w_vec2( vertices[ ( v + 1 ) % vertexCount ] ).from_b2d();

		RENDER->draw_line( v0, v1 );
	}

	RENDER->end();
}

/// Draw a solid closed polygon provided in CCW order.
void w_physics_debug_draw::DrawSolidPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color )
{
	RENDER
		->begin()
		->push_rgba( { color.r, color.g, color.b, color.a } );

	for( int v = 0 ; v < vertexCount ; ++v )
	{
		w_vec2 v0 = w_vec2( vertices[ v ] ).from_b2d();
		w_vec2 v1 = w_vec2( vertices[ ( v + 1 ) % vertexCount ] ).from_b2d();

		RENDER->draw_line( v0, v1 );
	}

	RENDER->end();
}

/// Draw a circle.
void w_physics_debug_draw::DrawCircle( const b2Vec2& center, float radius, const b2Color& color )
{
	w_vec2 position = w_vec2( center ).from_b2d();

	MATRIX
		->push()
		->translate( { position.x, position.y } );

	RENDER
		->begin()
		->push_rgba( w_color( color.r, color.g, color.b, color.a ) )
		->draw_circle( { 0.0f, 0.0f }, from_b2d( radius ) )
		->end();

	MATRIX->pop();
}

/// Draw a solid circle.
void w_physics_debug_draw::DrawSolidCircle( const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color )
{
	w_vec2 position = w_vec2( center ).from_b2d();

	MATRIX
		->push()
		->translate( { position.x, position.y } );

	RENDER
		->begin()
		->push_rgba( w_color( color.r, color.g, color.b, color.a ) )
		->draw_circle( { 0.0f, 0.0f }, from_b2d( radius ) )
		->end();

	MATRIX->pop();
}

/// Draw a line segment.
void w_physics_debug_draw::DrawSegment( const b2Vec2& p1, const b2Vec2& p2, const b2Color& color )
{
	w_vec2 v0 = w_vec2( p1 ).from_b2d();
	w_vec2 v1 = w_vec2( p2 ).from_b2d();

	RENDER
		->begin()
		->push_rgba( { color.r, color.g, color.b, color.a } )
		->draw_line( v0, v1 )
		->end();
}

/// Draw a transform. Choose your own length scale.
/// @param xf a transform.
void w_physics_debug_draw::DrawTransform( const b2Transform& xf )
{
	w_vec2 v = w_vec2( xf.p ).from_b2d();

	RENDER
		->begin()
		->push_rgba( w_color::white )
		->draw_point( v )
		->end();
}

/// Draw a point.
void w_physics_debug_draw::DrawPoint( const b2Vec2 & p, float size, const b2Color & color )
{
	w_vec2 v = w_vec2( p ).from_b2d();

	RENDER
		->begin()
		->push_rgba( { color.r, color.g, color.b, color.a } )
		->draw_point( v )
		->end();
}

// ----------------------------------------------------------------------------

void w_contact_listener::BeginContact( b2Contact* contact )
{
	this->contact = contact;
	manifold = contact->GetManifold();
	contact_ids[ 0 ] = (const char*) contact->GetFixtureA()->GetUserData().pointer;
	contact_ids[ 1 ] = (const char*) contact->GetFixtureB()->GetUserData().pointer;
}

void w_contact_listener::EndContact( b2Contact* contact )
{
	this->contact = contact;
	manifold = contact->GetManifold();
	contact_ids[ 0 ] = (const char*) contact->GetFixtureA()->GetUserData().pointer;
	contact_ids[ 1 ] = (const char*) contact->GetFixtureB()->GetUserData().pointer;
}

void w_contact_listener::PreSolve( b2Contact* contact, const b2Manifold* oldManifold )
{
	this->contact = contact;
	manifold = contact->GetManifold();
}

void w_contact_listener::PostSolve( b2Contact* contact, const b2ContactImpulse* impulse )
{
	this->contact = contact;
}

bool w_contact_listener::contact_ids_match( const char* id_0, const char* id_1 )
{
	return(
		( contact_ids[ 0 ] == id_0 && contact_ids[ 1 ] == id_1 )
		|| ( contact_ids[ 0 ] == id_1 && contact_ids[ 1 ] == id_0 )
		);
}

// looks at the 2 fixtures involved in this contact and returns the requested one.
b2Fixture* w_contact_listener::find_fixture_from_contact_id( const char* id )
{
	if( contact_ids[ 0 ] == id )
	{
		return contact->GetFixtureA();
	}
	if( contact_ids[ 1 ] == id )
	{
		return contact->GetFixtureB();
	}

	// contact id not found
	assert( false );

	return nullptr;
}

b2Body* w_contact_listener::find_body_from_contact_id( const char* id )
{
	return find_fixture_from_contact_id( id )->GetBody();
}

w_entity_component* w_contact_listener::find_component_from_contact_id( const char* id )
{
	return (w_entity_component*) ( find_body_from_contact_id( id )->GetUserData().pointer );
}

w_entity* w_contact_listener::find_entity_from_contact_id( const char* id )
{
	return find_component_from_contact_id( id )->parent_entity;
}

w_vec2 w_contact_listener::calc_hit_normal( b2Body* body_colliding )
{
	w_vec2 hit_normal = w_vec2::zero;

	if( manifold->type == 0 )	// circle
	{
		hit_normal = w_vec2( body_colliding->GetWorldCenter() ) - w_vec2( manifold->localPoint );
	}
	else
	{
		hit_normal = w_vec2( manifold->localNormal.x, manifold->localNormal.y );
	}

	return hit_normal.normalize();
}

// ----------------------------------------------------------------------------

bool w_physics::trace_simple( w_vec2 start, w_vec2 normal, float dist, bitflags layer_mask )
{
	w_raycast_simple callback;
	engine->box2d_world->RayCast( &callback, start.to_b2d(), ( start + ( normal * dist ) ).to_b2d() );

	return callback.hit_something;
}

bool w_physics::trace_simple( w_vec2 start, w_vec2 normal, float dist, bitflags layer_mask, w_raycast_simple* hit_result )
{
	engine->box2d_world->RayCast( hit_result, start.to_b2d(), ( start + ( normal * dist ) ).to_b2d() );

	return hit_result->hit_something;
}

bool w_physics::trace_closest( w_vec2 start, w_vec2 normal, float dist, bitflags layer_mask, w_raycast_closest* hit_result )
{
	engine->box2d_world->RayCast( hit_result, start.to_b2d(), ( start + ( normal * dist ) ).to_b2d() );
	return hit_result->hit_something;
}

bool w_physics::trace_all( w_vec2 start, w_vec2 normal, float dist, bitflags layer_mask, w_raycast_all* hit_result )
{
	engine->box2d_world->RayCast( hit_result, start.to_b2d(), ( start + ( normal * dist ) ).to_b2d() );
	return hit_result->hit_something;
}

bool w_physics::point_check_simple( w_vec2 pos, bitflags layer_mask )
{
	w_query_first hit_result;
	b2Vec2 bpos = pos.to_b2d();

	b2AABB aabb;
	aabb.lowerBound = bpos;
	aabb.upperBound = bpos;

	engine->box2d_world->QueryAABB( &hit_result, aabb );

	if( !hit_result.fixture->TestPoint( bpos ) )
	{
		return false;
	}

	return true;
}

bool w_physics::point_check_simple( w_vec2 pos, bitflags layer_mask, w_query_first* hit_result )
{
	b2Vec2 bpos = pos.to_b2d();

	b2AABB aabb;
	aabb.lowerBound = bpos;
	aabb.upperBound = bpos;

	engine->box2d_world->QueryAABB( hit_result, aabb );

	if( !hit_result->fixture->TestPoint( bpos ) )
	{
		hit_result->fixture = nullptr;
		return false;
	}

	return true;
}

bool w_physics::point_check_all( w_vec2 pos, bitflags layer_mask, w_query_all* hit_result )
{
	b2Vec2 bpos = pos.to_b2d();

	b2AABB aabb;
	aabb.lowerBound = bpos;
	aabb.upperBound = bpos;

	engine->box2d_world->QueryAABB( hit_result, aabb );

	std::vector<b2Fixture*> fixtures_hit;

	for( auto f : hit_result->fixtures )
	{
		if( f->TestPoint( bpos ) )
		{
			fixtures_hit.push_back( f );
		}
	}

	hit_result->fixtures = fixtures_hit;

	return ( hit_result->fixtures.size() > 0);
}

void w_physics::update()
{

}
