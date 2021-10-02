
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void Entity_Component::set_life_cycle( e_life_cycle lc )
{
	life_cycle.set( lc );
}

Entity_Component::Entity_Component( Entity* parent_entity )
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

bool Entity_Component::is_fully_dead()
{
	if( life_cycle.is_alive() or life_timer.has_value() )
	{
		return false;
	}

	return true;
}

void Entity_Component::draw()
{
}

void Entity_Component::pre_update()
{

}

void Entity_Component::update()
{
	if( life_timer.has_value() )
	{
		// if a life timer is being used AND that timer has elapsed, then this component is dying
		if( life_timer->is_elapsed() )
		{
			life_timer = std::nullopt;
			life_cycle.set( e_life_cycle::dying );
		}
	}

	// if this component is trying to die AND it meets the requirements to
	// be fully dead, then mark it dead

	if( life_cycle.is_dying() and is_fully_dead() )
	{
		life_cycle.set( e_life_cycle::dead );
	}
}

void Entity_Component::post_update()
{

}

void Entity_Component::play()
{
}

void Entity_Component::stop()
{
}

void Entity_Component::set_life_timer( int32_t life_in_ms )
{
	assert( !life_timer.has_value() );

	life_timer = Timer( life_in_ms );
}

void Entity_Component::set_collision_flags( int32_t collision_mask, int32_t collides_with )
{
	this->collision_mask = collision_mask;
	this->collides_with_mask = collides_with;
}

void Entity_Component::clear_collision_flags()
{
	collision_mask = collides_with_mask = 0;
}

// ----------------------------------------------------------------------------

Sprite_Component::Sprite_Component( Entity* parent_entity )
	: Entity_Component( parent_entity )
{
	anim_offset = Random::getf();
}

Entity_Component* Sprite_Component::init( std::string_view tex_tag )
{
	texture = g_engine->find_asset<Texture_Asset>( tex_tag );
	return this;
}

void Sprite_Component::draw()
{
	{
		scoped_opengl;

		auto mtx = g_engine->opengl_mgr.top_matrix;

		// flipping
		mtx->scale( { flip_x ? -1.f : 1.f, flip_y ? -1.f : 1.f } );

		scoped_render_state;

		Render::state->set_from_opt( rs_opt );
		Render::draw_sprite( texture, get_pos() );
	}
}

// ----------------------------------------------------------------------------

Primitive_Shape_Component::Primitive_Shape_Component( Entity* parent_entity )
	: Entity_Component( parent_entity )
{
	shapes.clear();
}

Entity_Component* Primitive_Shape_Component::add_shape( const e_primitive_shape prim_shape, const Rect& rc, const Vec2& pos_offset )
{
	Shape shape;

	shape.prim_shape = prim_shape;
	shape.rc = rc;
	shape.pos_offset = pos_offset;

	shapes.push_back( shape );

	return this;
}

Entity_Component* Primitive_Shape_Component::add_shape( const e_primitive_shape prim_shape, float_t radius, const Vec2& pos_offset )
{
	Shape shape;

	shape.prim_shape = prim_shape;
	shape.radius = radius;
	shape.pos_offset = pos_offset;

	shapes.push_back( shape );

	return this;
}

Entity_Component* Primitive_Shape_Component::add_shape( const e_primitive_shape prim_shape, const Vec2& pos_offset )
{
	Shape shape;

	shape.prim_shape = prim_shape;
	shape.pos_offset = pos_offset;

	shapes.push_back( shape );

	return this;
}

void Primitive_Shape_Component::draw()
{
	{
		scoped_render_state;

		Render::state->set_from_opt( rs_opt );

		for( auto& shape : shapes )
		{
			switch( shape.prim_shape )
			{
				case e_primitive_shape::filled_rect:
				{
					Render::draw_filled_rect( shape.rc + shape.pos_offset );
					break;
				}

				case e_primitive_shape::rect:
				{
					Render::draw_rect( shape.rc + shape.pos_offset );
					break;
				}

				case e_primitive_shape::circle:
				{
					Render::draw_circle( shape.pos_offset, shape.radius );
					break;
				}

				case e_primitive_shape::filled_circle:
				{
					Render::draw_filled_circle( shape.pos_offset, shape.radius );
					break;
				}

				case e_primitive_shape::point:
				{
					Render::draw_point( shape.pos_offset );
					break;
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------

Emitter_Component::Emitter_Component( Entity* parent_entity )
	: Entity_Component( parent_entity )
{
}

Entity_Component* Emitter_Component::init( std::string_view params_tag )
{
	emitter.set_params( g_engine->find_asset<Emitter_Parameters_Asset>( params_tag ) );
	emitter.parent_component = this;

	return this;
}

bool Emitter_Component::is_fully_dead()
{
	if( Entity_Component::is_fully_dead() )
	{
		if( life_cycle.is_dying() and emitter.pool.num_alive == 0 )
		{
			return true;
		}
	}

	return false;
}

void Emitter_Component::draw()
{
	{
		// particles live in world space, so remove any entity and component
		// level transforms before drawing the particle pool.
		//
		// this is what allows them to move through the world correctly once
		// spawned and be independent of the entity/emitter that spawned them.
		scoped_opengl_identity;

		scoped_render_state;

		Render::state->set_from_opt( rs_opt );
		emitter.pool.draw();
	}

}

void Emitter_Component::update()
{
	Entity_Component::update();

	if( !was_finalized )
	{
		was_finalized = true;

		if( emitter.params->needs_warm_up )
		{
			// particle warm ups require the parent and component transforms to be applied
			// so the warmed up particles spawn at the right position in the world.

			{
				scoped_opengl_identity;

				g_engine->opengl_mgr.top_matrix->apply_transform( *parent_entity->get_transform() );
				g_engine->opengl_mgr.top_matrix->apply_transform( *get_transform() );

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

void Emitter_Component::set_life_cycle( e_life_cycle lc )
{
	life_cycle.set( lc );

	if( life_cycle.is_dying() )
	{
		// setting the max to zero will cause the emitter to stop spawning new particles.
		emitter.max_particles_alive = 0;
	}
}

// ----------------------------------------------------------------------------

Sound_Component::Sound_Component( Entity* parent_entity )
	: Entity_Component( parent_entity )
{
}

Entity_Component* Sound_Component::init( std::string_view snd_tag, bool one_shot, bool auto_play )
{
	snd = g_engine->find_asset<Sound_Asset>( snd_tag );
	this->one_shot = one_shot;
	this->auto_play = auto_play;

	return this;
}

void Sound_Component::update()
{
	Entity_Component::update();

	if( auto_play )
	{
		auto_play = false;
		play();
	}
}

void Sound_Component::play()
{
	assert( snd );

	snd->play();

	if( one_shot )
	{
		life_cycle.set( e_life_cycle::dying );
	}
}

void Sound_Component::stop()
{
	assert( snd );

	snd->stop();
}

// ----------------------------------------------------------------------------

Box2D_Physics_Component::Box2D_Physics_Component( Entity* parent_entity )
	: Entity_Component( parent_entity )
{

}

void Box2D_Physics_Component::set_collision_flags( int32_t collision_mask, int32_t collides_with )
{
	Entity_Component::set_collision_flags( collision_mask, collides_with );

	// update all attached bodies to have matching flags

	auto ecs = parent_entity->get_components<Box2D_Physics_Body_Component>();

	for( auto ec : ecs )
	{
		ec->set_collision_flags( collision_mask, collides_with );
	}
}

void Box2D_Physics_Component::clear_collision_flags()
{
	collision_mask = 0;
	collides_with_mask = 0;
}

Box2D_Physics_Body_Component* Box2D_Physics_Component::get_primary_body()
{
	std::vector<Box2D_Physics_Body_Component*> ecs;
	parent_entity->get_components<Box2D_Physics_Body_Component, Box2D_Dynamic_Body_Component>( ecs );
	parent_entity->get_components<Box2D_Physics_Body_Component, Box2D_Kinematic_Body_Component>( ecs );

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
void Box2D_Physics_Component::set_friction( float_t friction )
{
	for( b2Fixture* fixture = get_primary_body()->body->GetFixtureList(); fixture; fixture = fixture->GetNext() )
	{
		fixture->SetFriction( friction );
	}
}

// restitution : 0 = no bounce, 1 = full bounce
void Box2D_Physics_Component::set_restitution( float_t restitution )
{
	for( b2Fixture* fixture = get_primary_body()->body->GetFixtureList(); fixture; fixture = fixture->GetNext() )
	{
		fixture->SetRestitution( restitution );
	}
}

// density : 0 = no density, 1 = full density
void Box2D_Physics_Component::set_density( float_t density )
{
	for( b2Fixture* fixture = get_primary_body()->body->GetFixtureList(); fixture; fixture = fixture->GetNext() )
	{
		fixture->SetDensity( density );
	}
}

// ----------------------------------------------------------------------------

Box2D_Physics_Body_Component::Box2D_Physics_Body_Component( Entity* parent_entity )
	: Entity_Component( parent_entity )
{
}

Box2D_Physics_Body_Component::~Box2D_Physics_Body_Component()
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

void Box2D_Physics_Body_Component::init_body()
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

b2Fixture* Box2D_Physics_Body_Component::add_fixture_box( hash tag, const Rect& rc )
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
		auto ecp = parent_entity->get_component<Box2D_Physics_Component>();

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
b2Fixture* Box2D_Physics_Body_Component::add_fixture_box( hash tag, Vec2 pos, float_t w, float_t h )
{
	return add_fixture_box( tag, { pos.x, pos.y, w, h } );
}

b2Fixture* Box2D_Physics_Body_Component::add_fixture_circle( hash tag, Vec2 pos, float_t radius )
{
	body->SetTransform( parent_entity->get_pos().to_box2d().to_b2Vec2(), 0.f );

	b2CircleShape shape;
	{
		shape.m_radius = to_box2d( radius );
		shape.m_p.Set( to_box2d( pos.x ), to_box2d( pos.y ) );
	}

	b2FixtureDef fixture_def;
	{
		auto ecp = parent_entity->get_component<Box2D_Physics_Component>();

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

b2Fixture* Box2D_Physics_Body_Component::add_fixture_line( hash tag, Vec2 pos, Vec2 start, Vec2 end )
{
	body->SetTransform( parent_entity->get_pos().to_box2d().to_b2Vec2(), 0.f );

	b2EdgeShape shape;
	{
		start += pos;
		end += pos;

		float_t length = ( end - start ).get_size() / 2.f;

		shape.SetOneSided(
			( start + Vec2( -length, length ) ).to_box2d().to_b2Vec2(),
			start.to_box2d().to_b2Vec2(),
			end.to_box2d().to_b2Vec2(),
			( end + Vec2( length, length ) ).to_box2d().to_b2Vec2()
		);
	}

	b2FixtureDef fixture_def;
	{
		auto ecp = parent_entity->get_component<Box2D_Physics_Component>();

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

b2Fixture* Box2D_Physics_Body_Component::add_fixture_line_loop( hash tag, Vec2 pos, const std::vector<Vec2>& verts )
{
	// Box2D requirement
	assert( verts.size() >= 3 );

	body->SetTransform( parent_entity->get_pos().to_box2d().to_b2Vec2(), 0.f );

	// convert the vertex list into a box2d friendly format
	std::vector<b2Vec2> b2verts;

	b2verts.reserve( verts.size() );
	for( Vec2 v : verts )
	{
		b2verts.push_back( ( v + pos ).to_box2d().to_b2Vec2() );
	}

	// we pass in the verts in a clockwise winding for compatibility with the
	// rest of the engine. box2d wants them wound counter-clockwise.

	std::reverse( b2verts.begin(), b2verts.end() );

	b2ChainShape shape;
	{
		shape.CreateLoop( b2verts.data(), (int32_t)( b2verts.size() ) );
	}

	b2FixtureDef fixture_def;
	{
		auto ecp = parent_entity->get_component<Box2D_Physics_Component>();

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

b2Fixture* Box2D_Physics_Body_Component::add_fixture_polygon( hash tag, Vec2 pos, const std::vector<Vec2>& verts )
{
	body->SetTransform( parent_entity->get_pos().to_box2d().to_b2Vec2(), 0.f );

	// convert the vertex list into a box2d friendly format
	std::vector<b2Vec2> b2verts;

	b2verts.reserve( verts.size() );
	for( Vec2 v : verts )
	{
		b2verts.push_back( ( v + pos ).to_box2d().to_b2Vec2() );
	}

	b2PolygonShape shape;
	{
		shape.Set( b2verts.data(), (int32_t)( b2verts.size() ) );
	}

	b2FixtureDef fixture_def;
	{
		auto ecp = parent_entity->get_component<Box2D_Physics_Component>();

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

void Box2D_Physics_Body_Component::add_physics_component_if_needed()
{
	if( !parent_entity->has_component<Box2D_Physics_Component>() )
	{
		parent_entity->add_component<Box2D_Physics_Component>();
	}
}

void Box2D_Physics_Body_Component::set_collision_flags( int32_t collision_mask, int32_t collides_with )
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

Box2D_Static_Body_Component::Box2D_Static_Body_Component( Entity* parent_entity )
	: Box2D_Physics_Body_Component( parent_entity )
{
	add_physics_component_if_needed();

	body_type = b2_staticBody;

	init_body();
}

// ----------------------------------------------------------------------------

Box2D_Dynamic_Body_Component::Box2D_Dynamic_Body_Component( Entity* parent_entity )
	: Box2D_Physics_Body_Component( parent_entity )
{
	add_physics_component_if_needed();

	body_type = b2_dynamicBody;

	init_body();
}

// ----------------------------------------------------------------------------

Box2D_Kinematic_Body_Component::Box2D_Kinematic_Body_Component( Entity* parent_entity )
	: Box2D_Physics_Body_Component( parent_entity )
{
	add_physics_component_if_needed();

	body_type = b2_kinematicBody;

	init_body();
}

// ----------------------------------------------------------------------------

Mesh_Component::Mesh_Component( Entity* parent_entity )
	: Entity_Component( parent_entity )
{
}

Entity_Component* Mesh_Component::init( std::string_view mesh_tag )
{
	mesh = g_engine->find_asset<Mesh_Asset>( mesh_tag );
	return this;
}

void Mesh_Component::draw()
{
	{
		scoped_render_state;

		Render::state->set_from_opt( rs_opt );
		Render::draw_mesh( mesh );
	}
}

// ----------------------------------------------------------------------------

Simple_Collision_Body::Simple_Collision_Body( Entity* parent_entity )
	: Entity_Component( parent_entity )
{
	sensor.time_next = g_engine->clock.now();
}

void Simple_Collision_Body::set_sensor_as_one_shot()
{
	solidity = e_solidity::sensor;
	sensor.type = e_sensor_type::one_shot;
}

void Simple_Collision_Body::set_sensor_as_repeating( time_ms delay )
{
	solidity = e_solidity::sensor;
	sensor.type = e_sensor_type::repeating;
	sensor.time_delay = delay;
	sensor.time_next = g_engine->clock.now();
}

void Simple_Collision_Body::set_sensor_as_continuous()
{
	solidity = e_solidity::sensor;
	sensor.type = e_sensor_type::continuous;
}

void Simple_Collision_Body::draw()
{
#ifndef _FINAL_RELEASE
	// optional debug mode drawing

	if( g_engine->render.debug.draw_debug_info )
	{
		scoped_render_state;

		Render::state->set_from_opt( rs_opt );

		// determine the color from the type

		switch( solidity )
		{
			case e_solidity::solid:
			{
				Render::state->color = make_color( Color::light_green );

				if( parent_entity->simple.is_stationary() )
				{
					Render::state->color = make_color( Color::green );
				}

				if( parent_entity->simple.is_kinematic() )
				{
					Render::state->color = make_color( Color::teal );
				}

				if( is_platform )
				{
					Render::state->color = make_color( Color::magenta );
				}
			}
			break;

			case e_solidity::sensor:
			{
				Render::state->color = make_color( Color::light_blue );
			}
			break;
		}

		// draw the primitive

		switch( prim_type )
		{
			case e_sc_prim_type::circle:
			{
				Render::draw_circle( { 0.f, 0.f }, radius );
			}
			break;

			case e_sc_prim_type::aabb:
			{
				Render::draw_rect( aabb );
			}
			break;

			case e_sc_prim_type::polygon:
			{
				Render::draw_lines( verts );
			}
			break;
		}
	}
#endif
}

void Simple_Collision_Body::update_to_match_parent_transform()
{
	auto scale = ( parent_entity->get_scale() * get_scale() );

	scoped_opengl;

	g_engine->opengl_mgr.top_matrix->identity();

	auto e_tform = parent_entity->get_transform();
	g_engine->opengl_mgr.top_matrix->apply_transform( e_tform->pos, e_tform->angle, e_tform->scale );

	auto ec_tform = get_transform();
	g_engine->opengl_mgr.top_matrix->apply_transform( ec_tform->pos, 0.f, ec_tform->scale );

	auto e_matrix = e_tform->to_matrix();
	auto ec_matrix = ec_tform->to_matrix();

	switch( prim_type )
	{
		case e_sc_prim_type::circle:
		{
			ws.pos = g_engine->opengl_mgr.top_matrix->transform_vec2( Vec2( 0.f, 0.f ) );
			ws.radius = radius * scale;

			auto r = ws.radius;
			ws.aabb = Rect( ws.pos.x - r, ws.pos.y - r, r * 2.f, r * 2.f );
		}
		break;

		case e_sc_prim_type::aabb:
		{
			auto v = g_engine->opengl_mgr.top_matrix->transform_vec2( Vec2( aabb.x, aabb.y ) );
			ws.aabb.x = v.x;
			ws.aabb.y = v.y;
			ws.aabb.w = aabb.w * scale;
			ws.aabb.h = aabb.h * scale;
		}
		break;

		case e_sc_prim_type::polygon:
		{
			Bounding_Box ws_aabb, os_aabb;

			ws.verts.clear();

			for( auto& v : verts )
			{
				auto ws_v = g_engine->opengl_mgr.top_matrix->transform_vec2( v );
				ws.verts.push_back( ws_v );

				ws_aabb.add( ws_v );

				os_aabb.add( v );
			}

			ws.aabb = ws_aabb.as_rect();
			ws.aabb.y -= ws.aabb.h;
		}
		break;
	}
}

// sets the dimensions of the collision box, with the component position being the top left corner..

void Simple_Collision_Body::set_as_box( float_t w, float_t h )
{
	prim_type = e_sc_prim_type::aabb;
	aabb.x = 0.f;
	aabb.y = 0.f;
	aabb.w = w;
	aabb.h = h;
}

// sets the dimensions of the collision box, centered around the components position.

void Simple_Collision_Body::set_as_centered_box( float_t w, float_t h )
{
	prim_type = e_sc_prim_type::aabb;
	aabb.x = -w / 2.f;
	aabb.y = -h / 2.f;
	aabb.w = w;
	aabb.h = h;
}

void Simple_Collision_Body::set_as_circle( float_t r )
{
	prim_type = e_sc_prim_type::circle;
	radius = r;
}

void Simple_Collision_Body::set_as_polygon( std::vector<Vec2> vs )
{
	prim_type = e_sc_prim_type::polygon;
	verts.clear();
	verts.reserve( verts.size() );
	verts.insert( verts.end(), vs.begin(), vs.end() );
}

// does a broad phase check against "scc" to see if these bodies are intersecting

bool Simple_Collision_Body::does_intersect( Simple_Collision_Body* scc )
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

	switch( prim_type )
	{
		case e_sc_prim_type::circle:
		{
			switch( scc->prim_type )
			{
				case e_sc_prim_type::circle:
				{
					// circle to circle

					c2Circle circle_a = as_simple_circle();
					c2Circle circle_b = scc->as_simple_circle();

					return c2CircletoCircle( circle_a, circle_b );
				}
				break;

				case e_sc_prim_type::aabb:
				{
					// circle to aabb

					c2Circle circle_a = as_simple_circle();
					c2AABB aabb_ws_b = scc->as_simple_aabb();

					return c2CircletoAABB( circle_a, aabb_ws_b );
				}
				break;

				case e_sc_prim_type::polygon:
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

		case e_sc_prim_type::aabb:
		{
			switch( scc->prim_type )
			{
				case e_sc_prim_type::circle:
				{
					// aabb to circle

					c2Circle circle_b = scc->as_simple_circle();
					c2AABB aabb_ws_a = as_simple_aabb();

					return c2CircletoAABB( circle_b, aabb_ws_a );
				}
				break;

				case e_sc_prim_type::aabb:
				{
					// aabb to aabb

					c2AABB aabb_ws_a = as_simple_aabb();
					c2AABB aabb_ws_b = scc->as_simple_aabb();

					return c2AABBtoAABB( aabb_ws_a, aabb_ws_b );
				}
				break;

				case e_sc_prim_type::polygon:
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

		case e_sc_prim_type::polygon:
		{
			switch( scc->prim_type )
			{
				case e_sc_prim_type::circle:
				{
					// polygon to circle

					c2Circle circle_b = scc->as_simple_circle();
					c2Poly poly_a = as_simple_poly();

					return c2CircletoPoly( circle_b, &poly_a, nullptr );
				}
				break;

				case e_sc_prim_type::aabb:
				{
					// polygon to aabb

					c2AABB aabb_ws_b = scc->as_simple_aabb();
					c2Poly poly_a = as_simple_poly();

					return c2AABBtoPoly( aabb_ws_b, &poly_a, nullptr );
				}
				break;

				case e_sc_prim_type::polygon:
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

std::optional<war::simple_collision::Pending_Collision> Simple_Collision_Body::intersects_with_manifold( Simple_Collision_Body* other )
{
	simple_collision::Pending_Collision collision;

	switch( prim_type )
	{
		case e_sc_prim_type::circle:
		{
			switch( other->prim_type )
			{
				case e_sc_prim_type::circle:
				{
					// circle to circle

					c2Circle circle_a = as_simple_circle();
					c2Circle circle_b = other->as_simple_circle();

					c2CircletoCircleManifold( circle_a, circle_b, &collision.manifold );
				}
				break;

				case e_sc_prim_type::aabb:
				{
					// circle to aabb

					c2Circle circle_a = as_simple_circle();
					c2AABB aabb_ws_b = other->as_simple_aabb();

					c2CircletoAABBManifold( circle_a, aabb_ws_b, &collision.manifold );
				}
				break;

				case e_sc_prim_type::polygon:
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

		case e_sc_prim_type::aabb:
		{
			switch( other->prim_type )
			{
				case e_sc_prim_type::circle:
				{
					// aabb to circle

					c2Circle circle_b = other->as_simple_circle();
					c2AABB aabb_ws_a = as_simple_aabb();

					c2CircletoAABBManifold( circle_b, aabb_ws_a, &collision.manifold );

					collision.manifold.n.x *= -1.f;
					collision.manifold.n.y *= -1.f;
				}
				break;

				case e_sc_prim_type::aabb:
				{
					// aabb to aabb

					c2AABB aabb_ws_a = as_simple_aabb();
					c2AABB aabb_ws_b = other->as_simple_aabb();

					c2AABBtoAABBManifold( aabb_ws_a, aabb_ws_b, &collision.manifold );
				}
				break;

				case e_sc_prim_type::polygon:
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

		case e_sc_prim_type::polygon:
		{
			switch( other->prim_type )
			{
				case e_sc_prim_type::circle:
				{
					// polygon to circle

					c2Circle circle_b = other->as_simple_circle();
					c2Poly poly_a = as_simple_poly();

					c2CircletoPolyManifold( circle_b, &poly_a, nullptr, &collision.manifold );

					collision.manifold.n.x *= -1.f;
					collision.manifold.n.y *= -1.f;
				}
				break;

				case e_sc_prim_type::aabb:
				{
					// polygon to aabb

					c2AABB aabb_ws_b = other->as_simple_aabb();
					c2Poly poly_a = as_simple_poly();

					c2AABBtoPolyManifold( aabb_ws_b, &poly_a, nullptr, &collision.manifold );

					collision.manifold.n.x *= -1.f;
					collision.manifold.n.y *= -1.f;
				}
				break;

				case e_sc_prim_type::polygon:
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

	// fill out the collision structure and return a positive result

	collision.entity_a = parent_entity;
	collision.body_a = this;

	collision.entity_b = other->parent_entity;
	collision.body_b = other;

	collision.closest_point =
		Vec2(
			collision.manifold.contact_points[ 0 ].x,
			collision.manifold.contact_points[ 0 ].y
		).from_simple();
	collision.normal = Vec2( collision.manifold.n.x, collision.manifold.n.y );
	collision.depth = from_simple( collision.manifold.depths[ 0 ] );

	return collision;
}

c2Circle Simple_Collision_Body::as_simple_circle()
{
	c2Circle circle = {};

	circle.p = { to_simple( ws.pos.x ), to_simple( ws.pos.y ) };
	circle.r = to_simple( ws.radius );

	return circle;
}

c2AABB Simple_Collision_Body::as_simple_aabb()
{
	return ws.aabb.to_c2AABB();
}

c2Poly Simple_Collision_Body::as_simple_poly()
{
	c2Poly poly = {};
	poly.count = (int32_t)ws.verts.size();

	for( auto x = 0 ; x < ws.verts.size() ; ++x )
	{
		poly.verts[ x ] = { to_simple( ws.verts[ x ].x ), to_simple( ws.verts[ x ].y ) };
	}

	c2MakePoly( &poly );

	return poly;
}

// ----------------------------------------------------------------------------

Simple_Collision_Platform_Body::Simple_Collision_Platform_Body( Entity* parent_entity )
	: Simple_Collision_Body( parent_entity )
{
	is_platform = true;
}

std::optional<war::simple_collision::Pending_Collision> Simple_Collision_Platform_Body::intersects_with_manifold( Simple_Collision_Body* other )
{
	// platforms have extra logic for rejecting collisions once they've been detected

	if( other->parent_entity->simple.is_dynamic() and other->parent_entity->velocity.y < 0.0f )
	{
		// the entity won't collide with the platform if it is currently
		// moving upwards

		return std::nullopt;
	}

	auto coll = Simple_Collision_Body::intersects_with_manifold( other );
	return coll;
}

// ----------------------------------------------------------------------------

Tile_Map_Component::Tile_Map_Component( Entity* parent_entity )
	: Entity_Component( parent_entity )
{
}

void Tile_Map_Component::init( std::string_view tile_set_tag, std::string_view tile_map_tag )
{
	tile_set = g_engine->find_asset<Tile_Set_Asset>( tile_set_tag );
	tile_map = g_engine->find_asset<Tile_Map_Asset>( tile_map_tag );

	// remove any existing collision components

	auto existing_components = parent_entity->get_components<Simple_Collision_Body>();

	for( auto& component : existing_components )
	{
		component->set_life_cycle( e_life_cycle::dying );
	}

	std::vector<Rect> aabbs;

	// Look through each tile and if a tile has collision data associated with it, add it into the pool.

	for( auto& layer : tile_map->layers )
	{
		if( !layer.flags.is_visible )
		{
			continue;
		}

		for( auto& chunk : layer.chunks )
		{
			for( auto y = 0 ; y < chunk.tilemap_bounds.h ; ++y )
			{
				for( auto x = 0 ; x < chunk.tilemap_bounds.w ; ++x )
				{
					auto tile = &( chunk.tiles[ ( y * (int32_t)chunk.tilemap_bounds.w ) + x ] );

					if( tile->idx == Tile_Map_Asset::Tile::empty )
					{
						continue;
					}

					Vec2 tile_pos =
					{
						( x + chunk.tilemap_bounds.x ) * tile_map->tile_sz,
						( y + chunk.tilemap_bounds.y ) * tile_map->tile_sz
					};

					auto tile_definition = &tile_set->tile_definitions[ tile->idx ];

					for( auto& obj : tile_definition->objects )
					{
						switch( obj.collision_prim_type )
						{
							case e_sc_prim_type::aabb:
							{
								aabbs.emplace_back( tile_pos.x + obj.rc.x, tile_pos.y + obj.rc.y, obj.rc.w, obj.rc.h );
							}
							break;

							case e_sc_prim_type::circle:
							{
								auto ec = parent_entity->add_component<Simple_Collision_Body>();
								ec->get_transform()->set_pos( { tile_pos.x + obj.rc.x + obj.radius, tile_pos.y + obj.rc.y + obj.radius } );
								ec->set_as_circle( obj.radius );
								ec->set_collision_flags( collision_mask, collides_with_mask );
							}
							break;

							case e_sc_prim_type::polygon:
							{
								auto ec = parent_entity->add_component<Simple_Collision_Body>();
								ec->get_transform()->set_pos( { tile_pos.x + obj.rc.x, tile_pos.y + obj.rc.y } );
								ec->set_as_polygon( obj.vertices );
								ec->set_collision_flags( collision_mask, collides_with_mask );
							}
							break;
						}
					}
				}
			}
		}
	}

	// merge together aabb collision boxes for perf reasons
	//
	// the merge routine is simplistic in that it merges boxes that are touching
	// each other and match in width or height. this could get more complicated
	// in the future by merging groups of aabbs and polygons into larger convex
	// polygons but it's unclear if it's worth it at this point.

	bool need_more_merging = true;

	while( need_more_merging )
	{
		need_more_merging = false;

		for( auto itera = 0 ; itera < aabbs.size(); ++itera )
		{
			Rect& recta = aabbs[ itera ];

			for( auto iterb = 0 ; iterb < aabbs.size(); ++iterb )
			{
				if( iterb == itera )
				{
					// can't merge with ourselves
					continue;
				}

				Rect& rectb = aabbs[ iterb ];

				// check horizontally

				if( fequals( recta.x + recta.w, rectb.x ) and fequals( recta.y, rectb.y ) and fequals( recta.h, rectb.h ) )
				{
					recta.w += rectb.w;
					aabbs.erase( aabbs.begin() + iterb );
					itera--;
					need_more_merging = true;
					break;
				}

				// check vertically

				if( fequals( recta.y + recta.h, rectb.y ) and fequals( recta.x, rectb.x ) and fequals( recta.w, rectb.w ) )
				{
					recta.h += rectb.h;
					aabbs.erase( aabbs.begin() + iterb );
					itera--;
					need_more_merging = true;
					break;
				}
			}
		}
	}

	// add the merged aabbs as collision components

	for( auto& rect : aabbs )
	{
		auto ec = parent_entity->add_component<Simple_Collision_Body>();
		ec->get_transform()->set_pos( { rect.x, rect.y } );
		ec->set_as_box( rect.w, rect.h );
		ec->set_collision_flags( collision_mask, collides_with_mask );
	}

	// create new collision components based on any objects attached to the tile
	// map itself
	//
	// these are collision primitives that were added manually in the
	// "collision" layer in tiled.

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
					// if a solid collider is tagged as a "platform", it's shape
					// is discarded and it's turned into a single line (aka a
					// box with no height)

					auto ec = parent_entity->add_component<Simple_Collision_Platform_Body>();
					ec->get_transform()->set_pos( { obj.rc.x, obj.rc.y } );
					ec->set_as_box( obj.rc.w, 0.f );
					ec->set_collision_flags( collision_mask, collides_with_mask );
				}
				else
				{
					add_collision_body_from_object( obj, e_solidity::solid );
				}
			}
		}
		else if( og.tag == "box2d_collision" )
		{
			// this doesn't exist yet - if you want it, we gotta write it!
			assert( false );
		}
		else if( og.tag == "sensors" )
		{
			for( auto& obj : og.objects )
			{
				auto ec = add_collision_body_from_object( obj, e_solidity::sensor );
				ec->set_sensor_as_one_shot();

				if( !obj.name.empty() )
				{
					// if the name field is populated with key/values, parse and
					// use them here

					Tokenizer tok( obj.name, "," );

					while( !tok.is_eos() )
					{
						Tokenizer kv( *tok.get_next_token(), "=" );

						auto key = kv.get_next_token();
						auto value = kv.get_next_token();

						if( *key == "tag" )
						{
							auto& sv = value.value();
							ec->tag = H( std::string( sv ).c_str() );
						}
						else if( *key == "delay" )
						{
							std::string wk = std::string( *value );
							String_Util::erase_char( wk, '\"' );
							ec->set_sensor_as_repeating( Text_Parser::uint_from_str( wk ) );
						}
					}
				}
			}
		}
		else
		{
			// unknown collision type
			assert( false );
		}
	}
}

// adds an appropriate simple_collision_body based on a Tiled_Object we read
// from the tile map

Simple_Collision_Body* Tile_Map_Component::add_collision_body_from_object( const Tiled_Object& obj, e_solidity collider_type )
{
	auto ec = parent_entity->add_component<Simple_Collision_Body>();

	switch( obj.collision_prim_type )
	{
		case e_sc_prim_type::aabb:
		{
			ec->get_transform()->set_pos( { obj.rc.x, obj.rc.y } );
			ec->set_as_box( obj.rc.w, obj.rc.h );
		}
		break;

		case e_sc_prim_type::circle:
		{
			ec->get_transform()->set_pos( { obj.rc.x + obj.radius, obj.rc.y + obj.radius } );
			ec->set_as_circle( obj.radius );
		}
		break;

		case e_sc_prim_type::polygon:
		{
			ec->get_transform()->set_pos( { obj.rc.x, obj.rc.y } );
			ec->set_as_polygon( obj.vertices );
		}
		break;
	}

	ec->set_collision_flags( collision_mask, collides_with_mask );

	return ec;
}

// loops through any layer with the tag "entities" and calls the callback
// function with that index so the layer in question can spawn it

void Tile_Map_Component::spawn_entities( Scene* scene, f_tile_map_spawn_entity func_callback )
{
	for( auto& layer : tile_map->layers )
	{
		if( layer.tag == H( "entities" ) )
		{
			for( auto& chunk : layer.chunks )
			{
				for( auto& tile : chunk.tiles )
				{
					if( tile.idx != Tile_Map_Asset::Tile::empty )
					{
						func_callback( scene, tile_map, &chunk, &tile );
					}
				}
			}
		}
	}
}

void Tile_Map_Component::draw()
{
	scoped_render_state;

	Render::draw_tile_map( tile_set, tile_map, Vec2( 0.f, 0.f ) );
}

}
