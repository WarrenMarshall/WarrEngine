
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void entity_component::set_life_cycle( e_life_cycle lc )
{
	life_cycle.set( lc );
}

entity_component::entity_component( entity* parent_entity )
	: parent_entity( parent_entity )
{
}

// each component can implement it's own criteria here for whether or not it is
// fully dead or not.
//
// components can sometimes have more work left to do before their owning
// entities can be safely deleted.
//
// reasons to return false from here might include:
//
// - you are emitting particles and need to wait until they are all faded out
//   before letting yourself get deleted.
// - you are waiting for a sound effect to finish playing
// - etc

bool entity_component::is_fully_dead()
{
	if( life_cycle.is_alive() or life_timer.has_value() )
	{
		return false;
	}

	return true;
}

void entity_component::draw()
{
}

void entity_component::update()
{
	if( life_timer.has_value() )
	{
		// if a life timer is being used AND that timer has elapsed, then this component is dying
		if( life_timer->is_elapsed() )
		{
			life_timer = std::nullopt;
			life_cycle.set( life_cycle::dying );
		}
	}

	// if this component is trying to die AND it meets the requirements to
	// be fully dead, then mark it dead

	if( life_cycle.is_dying() and is_fully_dead() )
	{
		life_cycle.set( life_cycle::dead );
	}
}

void entity_component::play()
{
}

void entity_component::stop()
{
}

void entity_component::set_life_timer( int life_in_ms )
{
	assert( !life_timer.has_value() );

	life_timer = timer( life_in_ms );
}

void entity_component::set_collision_flags( int collision_mask, int collides_with )
{
	this->collision_mask = collision_mask;
	this->collides_with_mask = collides_with;
}

void entity_component::clear_collision_flags()
{
	collision_mask = collides_with_mask = 0;
}

// ----------------------------------------------------------------------------

sprite_component::sprite_component( entity* parent_entity )
	: entity_component( parent_entity )
{
	anim_offset = random::getf();
}

entity_component* sprite_component::init( std::string_view tex_tag )
{
	texture = g_engine->find_asset<texture_asset>( tex_tag );
	return this;
}

void sprite_component::draw()
{
	{
		scoped_opengl;

		// flipping
		g_engine->render_api.top_matrix
			->scale( { flip_x ? -1.f : 1.f, flip_y ? -1.f : 1.f } );

		scoped_render_state;

		render::state->set_from_opt( rs_opt );
		render::draw_sprite( texture, get_transform()->pos );
	}
}

// ----------------------------------------------------------------------------

primitive_shape_component::primitive_shape_component( entity* parent_entity )
	: entity_component( parent_entity )
{
	shapes.clear();
}

entity_component* primitive_shape_component::add_shape( const e_primitive_shape prim_shape, const rect& rc )
{
	shape_def shape;

	shape.prim_shape = prim_shape;
	shape.rc = rc;

	shapes.emplace_back( shape );

	return this;
}

entity_component* primitive_shape_component::add_shape( const e_primitive_shape prim_shape, float radius )
{
	shape_def shape;

	shape.prim_shape = prim_shape;
	shape.radius = radius;

	shapes.emplace_back( shape );

	return this;
}

entity_component* primitive_shape_component::add_shape( const e_primitive_shape prim_shape )
{
	shape_def shape;

	shape.prim_shape = prim_shape;

	shapes.emplace_back( shape );

	return this;
}

void primitive_shape_component::draw()
{
	{
		scoped_render_state;

		render::state->set_from_opt( rs_opt );

		for( auto& shape : shapes )
		{
			switch( shape.prim_shape )
			{
				case primitive_shape::filled_rect:
				{
					render::draw_filled_rect( shape.rc );
					break;
				}

				case primitive_shape::rect:
				{
					render::draw_rect( shape.rc );
					break;
				}

				case primitive_shape::circle:
				{
					render::draw_circle( vec2( 0, 0 ), shape.radius );
					break;
				}

				case primitive_shape::filled_circle:
				{
					render::draw_filled_circle( vec2( 0, 0 ), shape.radius );
					break;
				}

				case primitive_shape::point:
				{
					render::draw_point( vec2( 0, 0 ) );
					break;
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------

emitter_component::emitter_component( entity* parent_entity )
	: entity_component( parent_entity )
{
}

entity_component* emitter_component::init( std::string_view params_tag )
{
	emitter.set_params( g_engine->find_asset<emitter_parameters_asset>( params_tag ) );
	emitter.parent_component = this;

	return this;
}

bool emitter_component::is_fully_dead()
{
	if( entity_component::is_fully_dead() )
	{
		if( life_cycle.is_dying() and emitter.pool.num_alive == 0 )
		{
			return true;
		}
	}

	return false;
}

void emitter_component::draw()
{
	{
		// particles live in world space, so remove any entity and component
		// level transforms before drawing the particle pool.
		//
		// this is what allows them to move through the world correctly once
		// spawned and be independent of the entity/emitter that spawned them.
		scoped_opengl_identity;

		scoped_render_state;

		render::state->set_from_opt( rs_opt );
		emitter.pool.draw();
	}

}

void emitter_component::update()
{
	entity_component::update();

	if( !was_finalized )
	{
		was_finalized = true;

		if( emitter.params->needs_warm_up )
		{
			// particle warm ups require the parent and component transforms to be applied
			// so the warmed up particles spawn at the right position in the world.

			{
				scoped_opengl_identity;

				g_engine->render_api.top_matrix->apply_transform( *parent_entity->get_transform() );
				g_engine->render_api.top_matrix->apply_transform( *get_transform() );

				emitter.warm_up();
			}
		}
		else
		{
			// the pool needs at least one update pass or it gets crashy
			emitter.pool.update();
		}
	}

	emitter.pool.update();
	emitter.update();
}

void emitter_component::set_life_cycle( e_life_cycle lc )
{
	life_cycle.set( lc );

	if( life_cycle.is_dying() )
	{
		// setting the max to zero will cause the emitter to stop spawning new particles.
		emitter.max_particles_alive = 0;
	}
}

// ----------------------------------------------------------------------------

sound_component::sound_component( entity* parent_entity )
	: entity_component( parent_entity )
{
}

entity_component* sound_component::init( std::string_view snd_tag, bool one_shot, bool auto_play )
{
	snd = g_engine->find_asset<sound_asset>( snd_tag );
	this->one_shot = one_shot;
	this->auto_play = auto_play;

	return this;
}

void sound_component::update()
{
	if( auto_play )
	{
		auto_play = false;
		play();
	}
}

void sound_component::play()
{
	assert( snd );

	snd->play();

	if( one_shot )
	{
		life_cycle.set( life_cycle::dying );
	}
}

void sound_component::stop()
{
	assert( snd );

	snd->stop();
}

// ----------------------------------------------------------------------------

box2d_physics_component::box2d_physics_component( entity* parent_entity )
	: entity_component( parent_entity )
{

}

void box2d_physics_component::set_collision_flags( int collision_mask, int collides_with )
{
	entity_component::set_collision_flags( collision_mask, collides_with );

	// update all attached bodies to have matching flags

	auto ecs = parent_entity->get_components<box2d_physics_body_component>();

	for( auto ec : ecs )
	{
		ec->set_collision_flags( collision_mask, collides_with );
	}
}

void box2d_physics_component::clear_collision_flags()
{
	collision_mask = 0;
	collides_with_mask = 0;
}

box2d_physics_body_component* box2d_physics_component::get_primary_body()
{
	std::vector<box2d_physics_body_component*> ecs;
	parent_entity->get_components<box2d_physics_body_component, box2d_dynamic_physics_body_component>( ecs );
	parent_entity->get_components<box2d_physics_body_component, box2d_kinematic_physics_body_component>( ecs );

	for( auto& ec : ecs )
	{
		if( ec->is_primary_body )
		{
			return ec;
		}
	}

	assert( false );	// no primary body found!
	return nullptr;
}

// friction : 0 - slide, 1 - stick
void box2d_physics_component::set_friction( float friction )
{
	for( b2Fixture* fixture = get_primary_body()->body->GetFixtureList(); fixture; fixture = fixture->GetNext() )
	{
		fixture->SetFriction( friction );
	}
}

// restitution : 0 = no bounce, 1 = full bounce
void box2d_physics_component::set_restitution( float restitution )
{
	for( b2Fixture* fixture = get_primary_body()->body->GetFixtureList(); fixture; fixture = fixture->GetNext() )
	{
		fixture->SetRestitution( restitution );
	}
}

// density : 0 = no density, 1 = full density
void box2d_physics_component::set_density( float density )
{
	for( b2Fixture* fixture = get_primary_body()->body->GetFixtureList(); fixture; fixture = fixture->GetNext() )
	{
		fixture->SetDensity( density );
	}
}

// ----------------------------------------------------------------------------

box2d_physics_body_component::box2d_physics_body_component( entity* parent_entity )
	: entity_component( parent_entity )
{
}

box2d_physics_body_component::~box2d_physics_body_component()
{
	if( body )
	{
		// save a list of fixtures attached to this body to a seperate list and destroy them.
		// this is the only safe way to destroy fixtures.

		std::basic_string<b2Fixture*> fixtures_to_delete;

		for( b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext() )
		{
			fixtures_to_delete += fixture;
		}

		for( auto f : fixtures_to_delete )
		{
			body->DestroyFixture( f );
		}

		// destroy the body itself

		g_engine->box2d_world->DestroyBody( body );
		body = nullptr;
	}
}

void box2d_physics_body_component::init_body()
{
	b2BodyDef body_definition;
	{
		body_definition.type = body_type;
		body_definition.position.Set( to_box2d( get_transform()->pos.x ), to_box2d( get_transform()->pos.y ) );
		body_definition.angle = 0.f;
	}

	body = g_engine->box2d_world->CreateBody( &body_definition );
	body->m_userData.pointer = (uintptr_t)this;
}

b2Fixture* box2d_physics_body_component::add_fixture_box( hash tag, const rect& rc )
{
	body->SetTransform( parent_entity->get_transform()->pos.to_box2d().to_b2Vec2(), 0.f );

	b2PolygonShape shape;
	{
		shape.SetAsBox(
			to_box2d( rc.w / 2 ),
			to_box2d( rc.h / 2 ),
			rc.get_midpoint().to_box2d().to_b2Vec2(),
			0.f
		);
	}

	b2FixtureDef fixture_def;
	{
		auto ecp = parent_entity->get_component<box2d_physics_component>();

		fixture_def.shape = &shape;
		fixture_def.density = 1.f;
		fixture_def.friction = 0.3f;
		fixture_def.restitution = 0.f;
		fixture_def.filter.categoryBits = (uint16)( ecp->collision_mask );
		fixture_def.filter.maskBits = (uint16)( ecp->collides_with_mask );
		fixture_def.userData.pointer = tag;
	}

	auto fixture = body->CreateFixture( &fixture_def );
	fixture_to_shape[ fixture ] = shape;
	return fixture;
}

// pos - middle of box, relative to body
// w/h - size of box
b2Fixture* box2d_physics_body_component::add_fixture_box( hash tag, vec2 pos, float w, float h )
{
	return add_fixture_box( tag, { pos.x, pos.y, w, h } );
}

b2Fixture* box2d_physics_body_component::add_fixture_circle( hash tag, vec2 pos, float radius )
{
	body->SetTransform( parent_entity->get_transform()->pos.to_box2d().to_b2Vec2(), 0.f );

	b2CircleShape shape;
	{
		shape.m_radius = to_box2d( radius );
		shape.m_p.Set( to_box2d( pos.x ), to_box2d( pos.y ) );
	}

	b2FixtureDef fixture_def;
	{
		auto ecp = parent_entity->get_component<box2d_physics_component>();

		fixture_def.shape = &shape;
		fixture_def.density = 1.f;
		fixture_def.friction = 0.3f;
		fixture_def.restitution = 0.f;
		fixture_def.filter.categoryBits = (uint16)( ecp->collision_mask );
		fixture_def.filter.maskBits = (uint16)( ecp->collides_with_mask );
		fixture_def.userData.pointer = tag;
	}

	auto fixture = body->CreateFixture( &fixture_def );
	fixture_to_shape[ fixture ] = shape;
	return fixture;
}

b2Fixture* box2d_physics_body_component::add_fixture_line( hash tag, vec2 pos, vec2 start, vec2 end )
{
	body->SetTransform( parent_entity->get_transform()->pos.to_box2d().to_b2Vec2(), 0.f );

	b2EdgeShape shape;
	{
		start += pos;
		end += pos;

		float length = ( end - start ).get_size_squared() / 2.f;

		shape.SetOneSided(
			( start + vec2( -length, length ) ).to_box2d().to_b2Vec2(),
			start.to_box2d().to_b2Vec2(),
			end.to_box2d().to_b2Vec2(),
			( end + vec2( length, length ) ).to_box2d().to_b2Vec2()
		);
	}

	b2FixtureDef fixture_def;
	{
		auto ecp = parent_entity->get_component<box2d_physics_component>();

		fixture_def.shape = &shape;
		fixture_def.density = 1.f;
		fixture_def.friction = 0.3f;
		fixture_def.restitution = 0.f;
		fixture_def.filter.categoryBits = (uint16)( ecp->collision_mask );
		fixture_def.filter.maskBits = (uint16)( ecp->collides_with_mask );
		fixture_def.userData.pointer = tag;
	}

	auto fixture = body->CreateFixture( &fixture_def );
	fixture_to_shape[ fixture ] = shape;
	return fixture;
}

b2Fixture* box2d_physics_body_component::add_fixture_line_loop( hash tag, vec2 pos, const std::vector<vec2>& verts )
{
	// Box2D requirement
	assert( verts.size() >= 3 );

	body->SetTransform( parent_entity->get_transform()->pos.to_box2d().to_b2Vec2(), 0.f );

	// convert the vertex list into a box2d friendly format
	std::vector<b2Vec2> b2verts;

	b2verts.reserve( verts.size() );
	for( vec2 v : verts )
	{
		b2verts.push_back( ( v + pos ).to_box2d().to_b2Vec2() );
	}

	// we pass in the verts in a clockwise winding for compatibility with the
	// rest of the engine. box2d wants them wound counter-clockwise.

	std::reverse( b2verts.begin(), b2verts.end() );

	b2ChainShape shape;
	{
		shape.CreateLoop( b2verts.data(), (int)( b2verts.size() ) );
	}

	b2FixtureDef fixture_def;
	{
		auto ecp = parent_entity->get_component<box2d_physics_component>();

		fixture_def.shape = &shape;
		fixture_def.density = 1.f;
		fixture_def.friction = 0.3f;
		fixture_def.restitution = 0.f;
		fixture_def.filter.categoryBits = (uint16)( ecp->collision_mask );
		fixture_def.filter.maskBits = (uint16)( ecp->collides_with_mask );
		fixture_def.userData.pointer = tag;
	}

	auto fixture = body->CreateFixture( &fixture_def );
	fixture_to_shape[ fixture ] = shape;
	return fixture;
}

b2Fixture* box2d_physics_body_component::add_fixture_polygon( hash tag, vec2 pos, const std::vector<vec2>& verts )
{
	body->SetTransform( parent_entity->get_transform()->pos.to_box2d().to_b2Vec2(), 0.f );

	// convert the vertex list into a box2d friendly format
	std::vector<b2Vec2> b2verts;

	b2verts.reserve( verts.size() );
	for( vec2 v : verts )
	{
		b2verts.push_back( ( v + pos ).to_box2d().to_b2Vec2() );
	}

	b2PolygonShape shape;
	{
		shape.Set( b2verts.data(), (int)( b2verts.size() ) );
	}

	b2FixtureDef fixture_def;
	{
		auto ecp = parent_entity->get_component<box2d_physics_component>();

		fixture_def.shape = &shape;
		fixture_def.density = 1.f;
		fixture_def.friction = 0.3f;
		fixture_def.restitution = 0.f;
		fixture_def.filter.categoryBits = (uint16)( ecp->collision_mask );
		fixture_def.filter.maskBits = (uint16)( ecp->collides_with_mask );
		fixture_def.userData.pointer = tag;
	}

	auto fixture = body->CreateFixture( &fixture_def );
	fixture_to_shape[ fixture ] = shape;
	return fixture;
}

// checks to see if the parent entity has a physics component, and adds one if
// it doesn't.

void box2d_physics_body_component::add_physics_component_if_needed()
{
	if( !parent_entity->has_component<box2d_physics_component>() )
	{
		parent_entity->add_component<box2d_physics_component>();
	}
}

void box2d_physics_body_component::set_collision_flags( int collision_mask, int collides_with )
{
	std::vector<b2Fixture*> existing_fixtures;

	for( b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext() )
	{
		existing_fixtures.push_back( fixture );
	}

	for( auto fixture : existing_fixtures )
	{
		b2FixtureDef fixture_def;

		auto& shape = fixture_to_shape[ fixture ];

		auto polygon_shape = std::get_if<b2PolygonShape>( &shape );
		auto circle_shape = std::get_if<b2CircleShape>( &shape );
		auto edge_shape = std::get_if<b2EdgeShape>( &shape );
		auto chain_shape = std::get_if<b2ChainShape>( &shape );

		if( polygon_shape )			fixture_def.shape = polygon_shape;
		else if( circle_shape )		fixture_def.shape = circle_shape;
		else if( edge_shape )		fixture_def.shape = edge_shape;
		else if( chain_shape )		fixture_def.shape = chain_shape;

		assert( fixture_def.shape );

		fixture_def.density = fixture->GetDensity();
		fixture_def.friction = fixture->GetFriction();
		fixture_def.restitution = fixture->GetRestitution();
		fixture_def.userData.pointer = fixture->GetUserData().pointer;

		fixture_def.filter.categoryBits = (uint16)( collision_mask );
		fixture_def.filter.maskBits = (uint16)( collides_with );

		auto new_fixture = body->CreateFixture( &fixture_def );

		if( polygon_shape )			fixture_to_shape[ new_fixture ] = *polygon_shape;
		else if( circle_shape )		fixture_to_shape[ new_fixture ] = *circle_shape;
		else if( edge_shape )		fixture_to_shape[ new_fixture ] = *edge_shape;
		else if( chain_shape )		fixture_to_shape[ new_fixture ] = *chain_shape;
	}

	for( auto fixture : existing_fixtures )
	{
		fixture_to_shape.erase( fixture );
		body->DestroyFixture( fixture );
	}
}

// ----------------------------------------------------------------------------

box2d_static_physics_body_component::box2d_static_physics_body_component( entity* parent_entity )
	: box2d_physics_body_component( parent_entity )
{
	add_physics_component_if_needed();

	body_type = b2_staticBody;

	init_body();
}

// ----------------------------------------------------------------------------

box2d_dynamic_physics_body_component::box2d_dynamic_physics_body_component( entity* parent_entity )
	: box2d_physics_body_component( parent_entity )
{
	add_physics_component_if_needed();

	body_type = b2_dynamicBody;

	init_body();
}

// ----------------------------------------------------------------------------

box2d_kinematic_physics_body_component::box2d_kinematic_physics_body_component( entity* parent_entity )
	: box2d_physics_body_component( parent_entity )
{
	add_physics_component_if_needed();

	body_type = b2_kinematicBody;

	init_body();
}

// ----------------------------------------------------------------------------

mesh_component::mesh_component( entity* parent_entity )
	: entity_component( parent_entity )
{
}

entity_component* mesh_component::init( std::string_view mesh_tag )
{
	mesh = g_engine->find_asset<mesh_asset>( mesh_tag );
	return this;
}

void mesh_component::draw()
{
	{
		scoped_render_state;

		render::state->set_from_opt( rs_opt );
		render::draw_mesh( mesh );
	}
}

// ----------------------------------------------------------------------------

simple_collision_component::simple_collision_component( entity* parent_entity )
	: entity_component( parent_entity )
{
}

entity_component* simple_collision_component::init( float w, float h )
{
	set_as_box( w, h );

	return this;
}

void simple_collision_component::draw()
{
	{
		scoped_render_state;

		render::state->set_from_opt( rs_opt );
		render::draw_rect( aabb );
	}
}

void simple_collision_component::update()
{
	entity_component::update();

	// transform the local space bounding box into worldspace coordinates. this
	// makes checking for collisions later on a lot simpler.

	transform pos_tform;
	pos_tform.add_pos( parent_entity->get_transform()->pos );
	pos_tform.add_pos( get_transform()->pos );
	auto pos_mtx = pos_tform.to_matrix();

	vec2 v;
	v = pos_mtx.transform_vec2( { aabb.x, aabb.y } );
	aabb_ws.x = v.x;
	aabb_ws.y = v.y;

	transform scale_tform;
	scale_tform.set_scale( parent_entity->get_transform()->scale );
	scale_tform.multiply_scale( get_transform()->scale );
	auto scale_mtx = scale_tform.to_matrix();

	v = scale_mtx.transform_vec2( { aabb.w, aabb.h } );
	aabb_ws.w = v.w;
	aabb_ws.h = v.h;

	aabb_ws.x -= aabb_ws.w / 2.0f;
	aabb_ws.y -= aabb_ws.h / 2.0f;
}

// sets the dimensions of the collision box, centered around the components position.

void simple_collision_component::set_as_box( float w, float h )
{
	aabb.x = -w / 2.f;
	aabb.y = -h / 2.f;
	aabb.w = w;
	aabb.h = h;
}

}
