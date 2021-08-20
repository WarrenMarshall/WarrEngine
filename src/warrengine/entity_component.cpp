
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

void entity_component::pre_update()
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

void entity_component::post_update()
{

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

ec_sprite::ec_sprite( entity* parent_entity )
	: entity_component( parent_entity )
{
	anim_offset = random::getf();
}

entity_component* ec_sprite::init( std::string_view tex_tag )
{
	texture = g_engine->find_asset<texture_asset>( tex_tag );
	return this;
}

void ec_sprite::draw()
{
	{
		scoped_opengl;

		// flipping
		g_engine->render_api.top_matrix
			->scale( { flip_x ? -1.f : 1.f, flip_y ? -1.f : 1.f } );

		scoped_render_state;

		render::state->set_from_opt( rs_opt );
		render::draw_sprite( texture, get_pos() );
	}
}

// ----------------------------------------------------------------------------

ec_primitive_shape::ec_primitive_shape( entity* parent_entity )
	: entity_component( parent_entity )
{
	shapes.clear();
}

entity_component* ec_primitive_shape::add_shape( const e_primitive_shape prim_shape, const rect& rc, const vec2& pos_offset )
{
	shape_def shape;

	shape.prim_shape = prim_shape;
	shape.rc = rc;
	shape.pos_offset = pos_offset;

	shapes.push_back( shape );

	return this;
}

entity_component* ec_primitive_shape::add_shape( const e_primitive_shape prim_shape, float radius, const vec2& pos_offset )
{
	shape_def shape;

	shape.prim_shape = prim_shape;
	shape.radius = radius;
	shape.pos_offset = pos_offset;

	shapes.push_back( shape );

	return this;
}

entity_component* ec_primitive_shape::add_shape( const e_primitive_shape prim_shape, const vec2& pos_offset )
{
	shape_def shape;

	shape.prim_shape = prim_shape;
	shape.pos_offset = pos_offset;

	shapes.push_back( shape );

	return this;
}

void ec_primitive_shape::draw()
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
					render::draw_filled_rect( shape.rc + shape.pos_offset );
					break;
				}

				case primitive_shape::rect:
				{
					render::draw_rect( shape.rc + shape.pos_offset );
					break;
				}

				case primitive_shape::circle:
				{
					render::draw_circle( shape.pos_offset, shape.radius );
					break;
				}

				case primitive_shape::filled_circle:
				{
					render::draw_filled_circle( shape.pos_offset, shape.radius );
					break;
				}

				case primitive_shape::point:
				{
					render::draw_point( shape.pos_offset );
					break;
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------

ec_emitter::ec_emitter( entity* parent_entity )
	: entity_component( parent_entity )
{
}

entity_component* ec_emitter::init( std::string_view params_tag )
{
	emitter.set_params( g_engine->find_asset<emitter_parameters_asset>( params_tag ) );
	emitter.parent_component = this;

	return this;
}

bool ec_emitter::is_fully_dead()
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

void ec_emitter::draw()
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

void ec_emitter::update()
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

void ec_emitter::set_life_cycle( e_life_cycle lc )
{
	life_cycle.set( lc );

	if( life_cycle.is_dying() )
	{
		// setting the max to zero will cause the emitter to stop spawning new particles.
		emitter.max_particles_alive = 0;
	}
}

// ----------------------------------------------------------------------------

ec_sound::ec_sound( entity* parent_entity )
	: entity_component( parent_entity )
{
}

entity_component* ec_sound::init( std::string_view snd_tag, bool one_shot, bool auto_play )
{
	snd = g_engine->find_asset<sound_asset>( snd_tag );
	this->one_shot = one_shot;
	this->auto_play = auto_play;

	return this;
}

void ec_sound::update()
{
	entity_component::update();

	if( auto_play )
	{
		auto_play = false;
		play();
	}
}

void ec_sound::play()
{
	assert( snd );

	snd->play();

	if( one_shot )
	{
		life_cycle.set( life_cycle::dying );
	}
}

void ec_sound::stop()
{
	assert( snd );

	snd->stop();
}

// ----------------------------------------------------------------------------

ec_box2d_physics::ec_box2d_physics( entity* parent_entity )
	: entity_component( parent_entity )
{

}

void ec_box2d_physics::set_collision_flags( int collision_mask, int collides_with )
{
	entity_component::set_collision_flags( collision_mask, collides_with );

	// update all attached bodies to have matching flags

	auto ecs = parent_entity->get_components<ec_box2d_physics_body>();

	for( auto ec : ecs )
	{
		ec->set_collision_flags( collision_mask, collides_with );
	}
}

void ec_box2d_physics::clear_collision_flags()
{
	collision_mask = 0;
	collides_with_mask = 0;
}

ec_box2d_physics_body* ec_box2d_physics::get_primary_body()
{
	std::vector<ec_box2d_physics_body*> ecs;
	parent_entity->get_components<ec_box2d_physics_body, ec_box2d_dynamic_physics_body>( ecs );
	parent_entity->get_components<ec_box2d_physics_body, ec_box2d_kinematic_physics_body>( ecs );

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
void ec_box2d_physics::set_friction( float friction )
{
	for( b2Fixture* fixture = get_primary_body()->body->GetFixtureList(); fixture; fixture = fixture->GetNext() )
	{
		fixture->SetFriction( friction );
	}
}

// restitution : 0 = no bounce, 1 = full bounce
void ec_box2d_physics::set_restitution( float restitution )
{
	for( b2Fixture* fixture = get_primary_body()->body->GetFixtureList(); fixture; fixture = fixture->GetNext() )
	{
		fixture->SetRestitution( restitution );
	}
}

// density : 0 = no density, 1 = full density
void ec_box2d_physics::set_density( float density )
{
	for( b2Fixture* fixture = get_primary_body()->body->GetFixtureList(); fixture; fixture = fixture->GetNext() )
	{
		fixture->SetDensity( density );
	}
}

// ----------------------------------------------------------------------------

ec_box2d_physics_body::ec_box2d_physics_body( entity* parent_entity )
	: entity_component( parent_entity )
{
}

ec_box2d_physics_body::~ec_box2d_physics_body()
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

		g_engine->box2d.world->DestroyBody( body );
		body = nullptr;
	}
}

void ec_box2d_physics_body::init_body()
{
	b2BodyDef body_definition;
	{
		body_definition.type = body_type;
		body_definition.position.Set( to_box2d( get_pos().x ), to_box2d( get_pos().y ) );
		body_definition.angle = 0.f;
	}

	body = g_engine->box2d.world->CreateBody( &body_definition );
	body->m_userData.pointer = (uintptr_t)this;
}

b2Fixture* ec_box2d_physics_body::add_fixture_box( hash tag, const rect& rc )
{
	body->SetTransform( parent_entity->get_pos().to_box2d().to_b2Vec2(), 0.f );

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
		auto ecp = parent_entity->get_component<ec_box2d_physics>();

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
b2Fixture* ec_box2d_physics_body::add_fixture_box( hash tag, vec2 pos, float w, float h )
{
	return add_fixture_box( tag, { pos.x, pos.y, w, h } );
}

b2Fixture* ec_box2d_physics_body::add_fixture_circle( hash tag, vec2 pos, float radius )
{
	body->SetTransform( parent_entity->get_pos().to_box2d().to_b2Vec2(), 0.f );

	b2CircleShape shape;
	{
		shape.m_radius = to_box2d( radius );
		shape.m_p.Set( to_box2d( pos.x ), to_box2d( pos.y ) );
	}

	b2FixtureDef fixture_def;
	{
		auto ecp = parent_entity->get_component<ec_box2d_physics>();

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

b2Fixture* ec_box2d_physics_body::add_fixture_line( hash tag, vec2 pos, vec2 start, vec2 end )
{
	body->SetTransform( parent_entity->get_pos().to_box2d().to_b2Vec2(), 0.f );

	b2EdgeShape shape;
	{
		start += pos;
		end += pos;

		float length = ( end - start ).get_size() / 2.f;

		shape.SetOneSided(
			( start + vec2( -length, length ) ).to_box2d().to_b2Vec2(),
			start.to_box2d().to_b2Vec2(),
			end.to_box2d().to_b2Vec2(),
			( end + vec2( length, length ) ).to_box2d().to_b2Vec2()
		);
	}

	b2FixtureDef fixture_def;
	{
		auto ecp = parent_entity->get_component<ec_box2d_physics>();

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

b2Fixture* ec_box2d_physics_body::add_fixture_line_loop( hash tag, vec2 pos, const std::vector<vec2>& verts )
{
	// Box2D requirement
	assert( verts.size() >= 3 );

	body->SetTransform( parent_entity->get_pos().to_box2d().to_b2Vec2(), 0.f );

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
		auto ecp = parent_entity->get_component<ec_box2d_physics>();

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

b2Fixture* ec_box2d_physics_body::add_fixture_polygon( hash tag, vec2 pos, const std::vector<vec2>& verts )
{
	body->SetTransform( parent_entity->get_pos().to_box2d().to_b2Vec2(), 0.f );

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
		auto ecp = parent_entity->get_component<ec_box2d_physics>();

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

void ec_box2d_physics_body::add_physics_component_if_needed()
{
	if( !parent_entity->has_component<ec_box2d_physics>() )
	{
		parent_entity->add_component<ec_box2d_physics>();
	}
}

void ec_box2d_physics_body::set_collision_flags( int collision_mask, int collides_with )
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

ec_box2d_static_physics_body::ec_box2d_static_physics_body( entity* parent_entity )
	: ec_box2d_physics_body( parent_entity )
{
	add_physics_component_if_needed();

	body_type = b2_staticBody;

	init_body();
}

// ----------------------------------------------------------------------------

ec_box2d_dynamic_physics_body::ec_box2d_dynamic_physics_body( entity* parent_entity )
	: ec_box2d_physics_body( parent_entity )
{
	add_physics_component_if_needed();

	body_type = b2_dynamicBody;

	init_body();
}

// ----------------------------------------------------------------------------

ec_box2d_kinematic_physics_body::ec_box2d_kinematic_physics_body( entity* parent_entity )
	: ec_box2d_physics_body( parent_entity )
{
	add_physics_component_if_needed();

	body_type = b2_kinematicBody;

	init_body();
}

// ----------------------------------------------------------------------------

ec_mesh::ec_mesh( entity* parent_entity )
	: entity_component( parent_entity )
{
}

entity_component* ec_mesh::init( std::string_view mesh_tag )
{
	mesh = g_engine->find_asset<mesh_asset>( mesh_tag );
	return this;
}

void ec_mesh::draw()
{
	{
		scoped_render_state;

		render::state->set_from_opt( rs_opt );
		render::draw_mesh( mesh );
	}
}

// ----------------------------------------------------------------------------

ec_simple_collision_body::ec_simple_collision_body( entity* parent_entity )
	: entity_component( parent_entity )
{
}

void ec_simple_collision_body::draw()
{
	// optional debug mode drawing

	if( g_engine->renderer.debug.draw_debug_info )
	{
		scoped_render_state;

		render::state->set_from_opt( rs_opt );

		// determine the color from the type

		switch( collider_type )
		{
			case sc_body_collider_type::solid:
			{
				render::state->color = make_color( color::light_green );

				if( parent_entity->simple.is_stationary() )
				{
					render::state->color = make_color( color::grey );
				}

				if( parent_entity->simple.is_kinematic() )
				{
					render::state->color = make_color( color::teal );
				}

				if( is_platform )
				{
					render::state->color = make_color( color::magenta );
				}
			}
			break;

			case sc_body_collider_type::sensor:
			{
				render::state->color = make_color( color::light_blue );
			}
			break;
		}

		// draw the primitive

		switch( type )
		{
			case sc_prim_type::circle:
			{
				render::draw_circle( { 0.f, 0.f }, radius );
			}
			break;

			case sc_prim_type::aabb:
			{
				render::draw_rect( aabb );
			}
			break;

			case sc_prim_type::polygon:
			{
				render::draw_lines( verts );
			}
			break;
		}
	}
}

void ec_simple_collision_body::update_to_match_parent_transform()
{
	auto scale = ( parent_entity->get_scale() * get_scale() );

	scoped_opengl;

	g_engine->render_api.top_matrix->identity();
	auto tform = parent_entity->get_transform();
	g_engine->render_api.top_matrix->apply_transform( tform->pos, tform->angle, tform->scale );
	tform = get_transform();
	g_engine->render_api.top_matrix->apply_transform( tform->pos, 0.f, tform->scale );

	switch( type )
	{
		case sc_prim_type::circle:
		{
			ws.pos = g_engine->render_api.top_matrix->transform_vec2( vec2( 0.f, 0.f ) );
			ws.radius = radius * scale;
		}
		break;

		case sc_prim_type::aabb:
		{
			auto v = g_engine->render_api.top_matrix->transform_vec2( vec2( aabb.x, aabb.y ) );
			ws.aabb.x = v.x;
			ws.aabb.y = v.y;
			ws.aabb.w = aabb.w * scale;
			ws.aabb.h = aabb.h * scale;
		}
		break;

		case sc_prim_type::polygon:
		{
			ws.verts.clear();
			for( auto& v : verts )
			{
				auto ws_v = g_engine->render_api.top_matrix->transform_vec2( v );
				ws.verts.push_back( ws_v );
			}
		}
		break;
	}
}

// sets the dimensions of the collision box, with the component position being the top left corner..

void ec_simple_collision_body::set_as_box( float w, float h )
{
	type = sc_prim_type::aabb;
	aabb.x = 0.f;
	aabb.y = 0.f;
	aabb.w = w;
	aabb.h = h;
}

// sets the dimensions of the collision box, centered around the components position.

void ec_simple_collision_body::set_as_centered_box( float w, float h )
{
	type = sc_prim_type::aabb;
	aabb.x = -w / 2.f;
	aabb.y = -h / 2.f;
	aabb.w = w;
	aabb.h = h;
}

void ec_simple_collision_body::set_as_circle( float r )
{
	type = sc_prim_type::circle;
	radius = r;
}

void ec_simple_collision_body::set_as_polygon( std::vector<vec2> vs )
{
	type = sc_prim_type::polygon;
	verts.clear();
	verts.reserve( verts.size() );
	verts.insert( verts.end(), vs.begin(), vs.end() );
}

void ec_simple_collision_body::set_body_collider_type( e_sc_body_collider_type type )
{
	this->collider_type = type;
}

// does a broad phase check against "scc" to see if these bodies are intersecting

bool ec_simple_collision_body::intersects_with( ec_simple_collision_body* scc )
{
	// simple_collision_components can't collide with themselves
	if( this == scc )
	{
		return false;
	}

	// entities can't collide with themselves
	if( this->parent_entity == scc->parent_entity )
	{
		return false;
	}

	// if collision masks don't intersect, skip the check
	if( !( this->collides_with_mask & scc->collision_mask ) )
	{
		return false;
	}

	// perform different checks based on body types

	switch( type )
	{
		case sc_prim_type::circle:
		{
			switch( scc->type )
			{
				case sc_prim_type::circle:
				{
					// circle to circle

					c2Circle circle_a = as_simple_circle();
					c2Circle circle_b = scc->as_simple_circle();

					return c2CircletoCircle( circle_a, circle_b );
				}
				break;

				case sc_prim_type::aabb:
				{
					// circle to aabb

					c2Circle circle_a = as_simple_circle();
					c2AABB aabb_ws_b = scc->as_simple_aabb();

					return c2CircletoAABB( circle_a, aabb_ws_b );
				}
				break;

				case sc_prim_type::polygon:
				{
					// circle to polygon

					c2Circle circle_a = as_simple_circle();
					c2Poly poly_b = scc->as_simple_poly();

					return c2CircletoPoly( circle_a, &poly_b, nullptr );
				}
				break;
			}
		}
		break;

		case sc_prim_type::aabb:
		{
			switch( scc->type )
			{
				case sc_prim_type::circle:
				{
					// aabb to circle

					c2Circle circle_b = scc->as_simple_circle();
					c2AABB aabb_ws_a = as_simple_aabb();

					return c2CircletoAABB( circle_b, aabb_ws_a );
				}
				break;

				case sc_prim_type::aabb:
				{
					// aabb to aabb

					c2AABB aabb_ws_a = as_simple_aabb();
					c2AABB aabb_ws_b = scc->as_simple_aabb();

					return c2AABBtoAABB( aabb_ws_a, aabb_ws_b );
				}
				break;

				case sc_prim_type::polygon:
				{
					// aabb to polygon

					c2AABB aabb_ws_a = as_simple_aabb();
					c2Poly poly_b = scc->as_simple_poly();

					return c2AABBtoPoly( aabb_ws_a, &poly_b, nullptr );
				}
				break;
			}
		}
		break;

		case sc_prim_type::polygon:
		{
			switch( scc->type )
			{
				case sc_prim_type::circle:
				{
					// polygon to circle

					c2Circle circle_b = scc->as_simple_circle();
					c2Poly poly_a = as_simple_poly();

					return c2CircletoPoly( circle_b, &poly_a, nullptr );
				}
				break;

				case sc_prim_type::aabb:
				{
					// polygon to aabb

					c2AABB aabb_ws_b = scc->as_simple_aabb();
					c2Poly poly_a = as_simple_poly();

					return c2AABBtoPoly( aabb_ws_b, &poly_a, nullptr );
				}
				break;

				case sc_prim_type::polygon:
				{
					// polygon to polygon

					c2Poly poly_a = as_simple_poly();
					c2Poly poly_b = scc->as_simple_poly();

					return c2PolytoPoly( &poly_a, nullptr, &poly_b, nullptr );
				}
				break;
			}
		}
		break;
	}

	return false;
}

// NOTE : this function assumes that this body and "other" ARE colliding.

std::optional<war::simple_collision::pending_collision> ec_simple_collision_body::intersects_with_manifold( ec_simple_collision_body* other )
{
	simple_collision::pending_collision collision;

	switch( type )
	{
		case sc_prim_type::circle:
		{
			switch( other->type )
			{
				case sc_prim_type::circle:
				{
					// circle to circle

					c2Circle circle_a = as_simple_circle();
					c2Circle circle_b = other->as_simple_circle();

					c2CircletoCircleManifold( circle_a, circle_b, &collision.manifold );
				}
				break;

				case sc_prim_type::aabb:
				{
					// circle to aabb

					c2Circle circle_a = as_simple_circle();
					c2AABB aabb_ws_b = other->as_simple_aabb();

					c2CircletoAABBManifold( circle_a, aabb_ws_b, &collision.manifold );
				}
				break;

				case sc_prim_type::polygon:
				{
					// circle to polygon

					c2Circle circle_a = as_simple_circle();
					c2Poly poly_b = other->as_simple_poly();

					c2CircletoPolyManifold( circle_a, &poly_b, nullptr, &collision.manifold );
				}
				break;
			}
		}
		break;

		case sc_prim_type::aabb:
		{
			switch( other->type )
			{
				case sc_prim_type::circle:
				{
					// aabb to circle

					c2Circle circle_b = other->as_simple_circle();
					c2AABB aabb_ws_a = as_simple_aabb();

					c2CircletoAABBManifold( circle_b, aabb_ws_a, &collision.manifold );

					collision.manifold.n.x *= -1.f;
					collision.manifold.n.y *= -1.f;
				}
				break;

				case sc_prim_type::aabb:
				{
					// aabb to aabb

					c2AABB aabb_ws_a = as_simple_aabb();
					c2AABB aabb_ws_b = other->as_simple_aabb();

					c2AABBtoAABBManifold( aabb_ws_a, aabb_ws_b, &collision.manifold );
				}
				break;

				case sc_prim_type::polygon:
				{
					// aabb to polygon

					c2AABB aabb_ws_a = as_simple_aabb();
					c2Poly poly_b = other->as_simple_poly();

					c2AABBtoPolyManifold( aabb_ws_a, &poly_b, nullptr, &collision.manifold );
				}
				break;
			}
		}
		break;

		case sc_prim_type::polygon:
		{
			switch( other->type )
			{
				case sc_prim_type::circle:
				{
					// polygon to circle

					c2Circle circle_b = other->as_simple_circle();
					c2Poly poly_a = as_simple_poly();

					c2CircletoPolyManifold( circle_b, &poly_a, nullptr, &collision.manifold );

					collision.manifold.n.x *= -1.f;
					collision.manifold.n.y *= -1.f;
				}
				break;

				case sc_prim_type::aabb:
				{
					// polygon to aabb

					c2AABB aabb_ws_b = other->as_simple_aabb();
					c2Poly poly_a = as_simple_poly();

					c2AABBtoPolyManifold( aabb_ws_b, &poly_a, nullptr, &collision.manifold );

					collision.manifold.n.x *= -1.f;
					collision.manifold.n.y *= -1.f;
				}
				break;

				case sc_prim_type::polygon:
				{
					// polygon to polygon

					c2Poly poly_a = as_simple_poly();
					c2Poly poly_b = other->as_simple_poly();

					c2PolytoPolyManifold( &poly_a, nullptr, &poly_b, nullptr, &collision.manifold );
				}
				break;
			}
		}
		break;

		default:
			assert( false );
			break;
	}

	// sometimes things go badly for reasons that are in the collision system
	// and not my code. when that happens, pretend like the collision didn't
	// happen because I don't see a better way to handle it.
	if( isnan( collision.manifold.depths[ 0 ] ) )
	{
		return std::nullopt;
	}

	// if any of these assert, something weird is happening
	//assert( !isnan( collision.manifold.n.x ) );
	//assert( !isnan( collision.manifold.n.y ) );

	// fill out the collision structure and return a positive result

	collision.entity_a = parent_entity;
	collision.body_a = this;

	collision.entity_b = other->parent_entity;
	collision.body_b = other;

	collision.closest_point =
		vec2(
			collision.manifold.contact_points[ 0 ].x,
			collision.manifold.contact_points[ 0 ].y
		).from_simple();
	collision.normal = vec2( collision.manifold.n.x, collision.manifold.n.y );
	collision.depth = from_simple( collision.manifold.depths[ 0 ] );

	return collision;
}

c2Circle ec_simple_collision_body::as_simple_circle()
{
	c2Circle circle = {};

	circle.p = { to_simple( ws.pos.x ), to_simple( ws.pos.y ) };
	circle.r = to_simple( ws.radius );

	return circle;
}

c2AABB ec_simple_collision_body::as_simple_aabb()
{
	return ws.aabb.to_c2AABB();
}

c2Poly ec_simple_collision_body::as_simple_poly()
{
	c2Poly poly = {};
	poly.count = (int)ws.verts.size();

	for( auto x = 0 ; x < ws.verts.size() ; ++x )
	{
		poly.verts[ x ] = { to_simple( ws.verts[ x ].x ), to_simple( ws.verts[ x ].y ) };
	}

	c2MakePoly( &poly );

	return poly;
}

// looks at the bounds of this body and returns a bounding circle

c2Circle ec_simple_collision_body::get_bounds_as_simple_circle()
{
	c2Circle circle = {};

	switch( type )
	{
		case sc_prim_type::circle:
		{
			circle = as_simple_circle();
		}
		break;

		case sc_prim_type::aabb:
		{
			assert( false );	// this has never been tested
			auto aabb_ws = as_simple_aabb();

			vec2 extents = { aabb_ws.max.x - aabb_ws.min.x, aabb_ws.max.y - aabb_ws.min.y };

			circle.p = { extents.x / 2.f, extents.y / 2.f };
			circle.r = glm::max( extents.x, extents.y );
		}
		break;

		case sc_prim_type::polygon:
		{
			assert( false );	// this has never been tested

			auto poly_ws = as_simple_poly();

			bounding_box bbox;
			for( int v = 0 ; v < poly_ws.count ; ++v )
			{
				bbox.add( { poly_ws.verts[ v ].x, poly_ws.verts[ v ].y } );
			}

			vec2 extents = { bbox.max.x - bbox.min.x, bbox.max.y - bbox.min.y };

			circle.p = { extents.x / 2.f, extents.y / 2.f };
			circle.r = glm::max( extents.x, extents.y );
		}
		break;
	}
	return circle;
}

// ----------------------------------------------------------------------------

ec_simple_collision_body_platform::ec_simple_collision_body_platform( entity* parent_entity )
	: ec_simple_collision_body( parent_entity )
{
	is_platform = true;
}

std::optional<war::simple_collision::pending_collision> ec_simple_collision_body_platform::intersects_with_manifold( ec_simple_collision_body* other )
{
	auto coll = ec_simple_collision_body::intersects_with_manifold( other );

	// sensors collide with platforms the same as they do regular bodies.

	if( coll->body_b->collider_type == sc_body_collider_type::sensor )
	{
		return coll;
	}

	auto oe = other->parent_entity;

	// platforms have extra logic for rejecting collisions once they've been detected

	if( oe->simple.is_dynamic() )
	{
		if( oe->velocity.y < 0.0f )
		{
			// the entity won't collide with the platform if it is currently
			// moving upwards

			return std::nullopt;
		}
	}

	return coll;
}

// ----------------------------------------------------------------------------

ec_tile_map::ec_tile_map( entity* parent_entity )
	: entity_component( parent_entity )
{
}

void ec_tile_map::init( std::string_view tile_set_tag, std::string_view tile_map_tag )
{
	tile_set = g_engine->find_asset<tile_set_asset>( tile_set_tag );
	tile_map = g_engine->find_asset<tile_map_asset>( tile_map_tag );

	// remove any existing collision components

	auto existing_components = parent_entity->get_components<ec_simple_collision_body>();

	for( auto& component : existing_components )
	{
		component->set_life_cycle( life_cycle::dying );
	}

	// create new collision components based on any objects attached to the tile map

	for( auto& og : tile_map->object_groups )
	{
		if( !og.is_visible )
		{
			continue;
		}

		if( og.tag == "collision" )
		{
			for( auto& obj : og.objects )
			{
				if( obj.type == "platform" )
				{
					auto ec = parent_entity->add_component<ec_simple_collision_body_platform>();
					ec->get_transform()->set_pos( { obj.rc.x, obj.rc.y } );
					ec->set_as_box( obj.rc.w, 0.f );
					ec->set_collision_flags( collision_mask, collides_with_mask );
				}
				else
				{
					switch( obj.collision_type )
					{
						case sc_prim_type::aabb:
						{
							auto ec = parent_entity->add_component<ec_simple_collision_body>();
							ec->get_transform()->set_pos( { obj.rc.x, obj.rc.y } );
							ec->set_as_box( obj.rc.w, obj.rc.h );
							ec->set_collision_flags( collision_mask, collides_with_mask );
						}
						break;

						case sc_prim_type::circle:
						{
							auto ec = parent_entity->add_component<ec_simple_collision_body>();
							ec->get_transform()->set_pos( { obj.rc.x + obj.radius, obj.rc.y + obj.radius } );
							ec->set_as_circle( obj.radius );
							ec->set_collision_flags( collision_mask, collides_with_mask );
						}
						break;

						case sc_prim_type::polygon:
						{
							auto ec = parent_entity->add_component<ec_simple_collision_body>();
							ec->get_transform()->set_pos( { obj.rc.x, obj.rc.y } );
							ec->set_as_polygon( obj.vertices );
							ec->set_collision_flags( collision_mask, collides_with_mask );
						}
						break;
					}
				}
			}
		}
		else if( og.tag == "box2d_collision" )
		{
			// this doesn't exist yet - if you want it, we gotta write it!
			assert( false );
		}
		else
		{
			// unknown collision type
			assert( false );
		}
	}
}

// loops through any layer with the name "entities" and calls the callback
// function with that index so the layer in question can spawn it

void ec_tile_map::spawn_entities( scene* scene, f_tile_map_spawn_entity func_callback )
{
	for( auto& layer : tile_map->layers )
	{
		if( layer.tag == "entities" )
		{
			for( auto& tile : layer.tiles )
			{
				if( tile.idx != tile_map_asset::tile::empty )
				{
					func_callback( scene, &tile );
				}
			}
		}
	}
}

void ec_tile_map::draw()
{
	scoped_render_state;

	render::draw_tile_map( tile_set, tile_map, vec2( 0.f, 0.f ) );
}

// ----------------------------------------------------------------------------

}
