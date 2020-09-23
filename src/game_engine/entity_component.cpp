
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_entity_component::w_entity_component( w_entity* parent_entity )
	: parent_entity( parent_entity )
{
	generic_offset = w_random::getf();
}

/*
	each component can implement it's own criteria here for
	whether or not it is fully dead or not.

	components can sometimes have more work left to do before
	their owning entities can be safely deleted.

	reasons to return false from here might include:

	- you are emitting particles and need to wait until they are all faded
	  out before letting yourself get deleted.
	- you are waiting for a sound effect to finish playing
	- etc
*/
bool w_entity_component::is_fully_dead()
{
	if( is_alive() || life_timer )
	{
		return false;
	}

	return true;
}

void w_entity_component::update()
{
	// if a timer is being used, and it has elapsed, then this component is dead
	if( life_timer )
	{
		life_timer->update();

		if( life_timer->is_elapsed() )
		{
			life_timer = nullptr;
			set_life_cycle( life_cycle::dying );
		}
	}

	// if this component is trying to die AND it meets the requirements to
	// be fully dead, then mark it dead

	if( is_dying() && is_fully_dead() )
	{
		set_life_cycle( life_cycle::dead );
	}

	pos_interp = w_vec2::zero;
}

void w_entity_component::set_life_timer( int life_in_ms )
{
	assert( life_timer == nullptr );

	life_timer = std::make_unique<w_timer>( life_in_ms );
}

// ----------------------------------------------------------------------------

ec_sprite::ec_sprite( w_entity* parent_entity )
	: w_entity_component( parent_entity )
{
	type |= component_type::sprite;
}

w_entity_component* ec_sprite::init( const std::string_view subtex_name )
{
	subtex = engine->get_asset<a_subtexture>( subtex_name );
	return this;
}

void ec_sprite::draw()
{
	if( is_dead() )
	{
		return;
	}

	// #box2d
	//w_vec2 pos_interp = w_vec2::multiply( parent_entity->physics_cache.forces, RENDER->frame_interpolate_pct );
	//RENDER->draw_sprite( subtex, w_rect( pos_interp.x, pos_interp.y ) );

	RENDER->draw_sprite( subtex, w_rect( pos.x, pos.y ) );
}

// ----------------------------------------------------------------------------

ec_emitter::ec_emitter( w_entity* parent_entity )
	: w_entity_component( parent_entity )
{
	type |= component_type::emitter;
}

w_entity_component* ec_emitter::init( const std::string_view params_name )
{
	emitter = std::make_unique<w_particle_emitter>();
	emitter->set_params( engine->get_asset<a_emitter_params>( params_name ) );
	emitter->parent_component = this;

	return this;
}

bool ec_emitter::is_fully_dead()
{
	if( w_entity_component::is_fully_dead() )
	{
		if( is_dying() && emitter->particle_pool->num_particles_alive == 0 )
		{
			return true;
		}
	}

	return false;
}

void ec_emitter::draw()
{
	if( is_dead() )
	{
		return;
	}

	// particles live in world space, so remove any entity and
	// component level transforms before drawing the particle pool

	MATRIX
		->push_identity();

	emitter->particle_pool->draw();

	MATRIX
		->pop();

}

void ec_emitter::update()
{
	w_entity_component::update();

	if( is_dead() )
	{
		return;
	}

	emitter->update();
	emitter->particle_pool->update();
}

void ec_emitter::set_life_cycle( e_life_cycle life_cycle )
{
	i_life_cycle::set_life_cycle( life_cycle );

	if( is_dying() )
	{
		// setting the max to zero will cause the emitter to stop spawning new particles.
		emitter->max_particles_alive = 0;
	}
}

void ec_emitter::post_spawn()
{
	emitter->post_spawn();
}

// ----------------------------------------------------------------------------

ec_sound::ec_sound( w_entity* parent_entity )
	: w_entity_component( parent_entity )
{
	type |= component_type::sound;
}

w_entity_component* ec_sound::init( const std::string_view snd_name )
{
	snd = engine->get_asset<a_sound>( snd_name );

	return this;
}

void ec_sound::draw()
{
	if( snd )
	{
		snd->play();
	}
	snd = nullptr;

	set_life_cycle( life_cycle::dying );
}

// ----------------------------------------------------------------------------

ec_b2d_body::ec_b2d_body( w_entity* parent_entity )
	: w_entity_component( parent_entity )
{
	type |= component_type::b2d_body;
}

ec_b2d_body::~ec_b2d_body()
{
	engine->box2d_world->DestroyBody( body );
}

void ec_b2d_body::init_body()
{
	b2BodyDef body_definition;
	{
		body_definition.type = body_type;
		body_definition.position.Set( to_b2d( pos.x ), to_b2d( pos.y ) );
		body_definition.angle = 0.0f;
	}

	body = engine->box2d_world->CreateBody( &body_definition );
	body->m_userData.pointer = (uintptr_t) this;
}

void ec_b2d_body::draw()
{
	w_entity_component::draw();

	if( !parent_entity->draw_debug_info )
	{
		return;
	}

	MATRIX->push_identity();

	for( b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext() )
	{
		if( fixture->IsSensor() )
		{
			RENDER->push_rgb( body->IsAwake() ? w_color::teal : w_color::dark_teal );
		}
		else
		{
			RENDER->push_rgb( body->IsAwake() ? w_color::green : w_color::dark_green );
		}

		b2Shape::Type shapeType = fixture->GetType();

		if( shapeType == b2Shape::e_circle )
		{
			auto* shape = (b2CircleShape*) fixture->GetShape();

			b2Vec2 position = body->GetPosition();
			position.x = from_b2d( position.x + shape->m_p.x );
			position.y = from_b2d( position.y + shape->m_p.y );

			float angle = body->GetAngle();

			MATRIX
				->push()
				->translate( { position.x, position.y } )
				->rotate( rad2deg( angle ) );

			RENDER->draw_circle( { 0.0f, 0.0f }, from_b2d( shape->m_radius ) );

			MATRIX->pop();
		}
		else if( shapeType == b2Shape::e_polygon )
		{
			b2Vec2 position = body->GetPosition();
			position.x = from_b2d( position.x );
			position.y = from_b2d( position.y );

			float angle = body->GetAngle();

			MATRIX
				->push()
				->translate( { position.x, position.y } )
				->rotate( rad2deg( angle ) );

			auto* shape = (b2PolygonShape*) fixture->GetShape();

			for( int v = 0 ; v < shape->m_count ; ++v )
			{
				b2Vec2 v0 = shape->m_vertices[ v ];
				v0.x = from_b2d( v0.x );
				v0.y = from_b2d( v0.y );

				b2Vec2 v1 = shape->m_vertices[ ( v + 1 ) % shape->m_count ];
				v1.x = from_b2d( v1.x );
				v1.y = from_b2d( v1.y );

				RENDER->draw_line( w_vec2( v0.x, v0.y ), w_vec2( v1.x, v1.y ) );
			}

			MATRIX->pop();
		}
		else if( shapeType == b2Shape::e_edge )
		{
			auto* shape = (b2EdgeShape*) fixture->GetShape();

			b2Vec2 position = body->GetPosition();
			position.x = from_b2d( position.x );
			position.y = from_b2d( position.y );

			float angle = body->GetAngle();

			MATRIX
				->push()
				->translate( { position.x, position.y } )
				->rotate( rad2deg( angle ) );

			b2Vec2 v1 = body->GetWorldPoint( shape->m_vertex1 );
			v1.x = from_b2d( v1.x );
			v1.y = from_b2d( v1.y );

			b2Vec2 v2 = body->GetWorldPoint( shape->m_vertex2 );
			v2.x = from_b2d( v2.x );
			v2.y = from_b2d( v2.y );

			RENDER->draw_line( { v1.x, v1.y }, { v2.x, v2.y } );

			MATRIX->pop();
		}
		else if( shapeType == b2Shape::e_chain )
		{
			b2Vec2 position = body->GetPosition();
			position.x = from_b2d( position.x );
			position.y = from_b2d( position.y );

			float angle = body->GetAngle();

			MATRIX
				->push()
				->translate( { position.x, position.y } )
				->rotate( rad2deg( angle ) );

			auto* shape = (b2ChainShape*) fixture->GetShape();

			for( int v = 0 ; v < shape->m_count ; ++v )
			{
				b2Vec2 v0 = shape->m_vertices[ v ];
				v0.x = from_b2d( v0.x );
				v0.y = from_b2d( v0.y );

				b2Vec2 v1 = shape->m_vertices[ ( v + 1 ) % shape->m_count ];
				v1.x = from_b2d( v1.x );
				v1.y = from_b2d( v1.y );

				RENDER->draw_line( w_vec2( v0.x, v0.y ), w_vec2( v1.x, v1.y ) );
			}

			MATRIX->pop();
		}
		else
		{
			log_error( "Unknown Box2D shape type" );
		}

		RENDER->pop_rgb();
	}

	MATRIX->pop();
}

// rc - the top left of the box (relative to body) and the w/h
b2Fixture* ec_b2d_body::add_fixture_box( unsigned id, w_rect rc )
{
	w_vec2 pos = { rc.x + ( *rc.w / 2.0f ), rc.y + ( *rc.h / 2.0f ) };

	return add_fixture_box( id, pos, *rc.w, *rc.h );
}

// pos - middle of box, relative to body
// w/h - size of box
b2Fixture* ec_b2d_body::add_fixture_box( unsigned id, w_vec2 pos, float w, float h )
{
	body->SetTransform( parent_entity->pos.to_b2d(), 0.0f );

	b2PolygonShape shape;
	{
		shape.SetAsBox(
			to_b2d( w / 2 ),
			to_b2d( h / 2 ),
			w_vec2( pos.x, pos.y ).to_b2d(),
			0.0f
		);
	}

	b2FixtureDef fixture;
	{
		fixture.shape = &shape;
		fixture.density = 1.0f;
		fixture.friction = 0.3f;
		fixture.filter.categoryBits = static_cast<uint16>( parent_entity->collision_layer );
		fixture.filter.maskBits = static_cast<uint16>( parent_entity->collides_with );
		fixture.userData.pointer = id;
	}

	return body->CreateFixture( &fixture );
}

b2Fixture* ec_b2d_body::add_fixture_circle( unsigned id, w_vec2 pos, float radius )
{
	body->SetTransform( parent_entity->pos.to_b2d(), 0.0f );

	b2CircleShape shape;
	{
		shape.m_radius = to_b2d( radius );
		shape.m_p.Set( to_b2d( pos.x ), to_b2d( pos.y ) );
	}

	b2FixtureDef fixture;
	{
		fixture.shape = &shape;
		fixture.density = 1.0f;
		fixture.friction = 0.3f;
		fixture.filter.categoryBits = static_cast<uint16>( parent_entity->collision_layer );
		fixture.filter.maskBits = static_cast<uint16>( parent_entity->collides_with );
		fixture.userData.pointer = id;
	}

	return body->CreateFixture( &fixture );
}

// #box2d - does this even work?  seems broken when I try to use it.
// line appears in a weird place and moves out of sync with entity it's attached to.
b2Fixture* ec_b2d_body::add_fixture_line( unsigned id, w_vec2 pos, w_vec2 start, w_vec2 end )
{
	body->SetTransform( parent_entity->pos.to_b2d(), 0.0f );

	b2EdgeShape shape;
	{
		shape.SetOneSided(
			( pos + start ).to_b2d(),
			( pos + start ).to_b2d(),
			( pos + end ).to_b2d(),
			( pos + end ).to_b2d()
		);
	}

	b2FixtureDef fixture;
	{
		fixture.shape = &shape;
		fixture.density = 1.0f;
		fixture.friction = 0.3f;
		fixture.filter.categoryBits = static_cast<uint16>( parent_entity->collision_layer );
		fixture.filter.maskBits = static_cast<uint16>( parent_entity->collides_with );
		fixture.userData.pointer = id;
	}

	return body->CreateFixture( &fixture );
}

b2Fixture* ec_b2d_body::add_fixture_chain( unsigned id, w_vec2 pos, const std::vector<w_vec2>& verts )
{
	body->SetTransform( parent_entity->pos.to_b2d(), 0.0f );

	// convert the vertex list into a box2d friendly format
	std::vector<b2Vec2> b2verts;

	for(w_vec2 v : verts)
	{
		b2verts.push_back( (v + pos ).to_b2d() );
	}

	// we pass in the verts in a clockwise winding for compatibility with the
	// rest of the engine. box2d wants them wound counter-clockwise.
	std::reverse( b2verts.begin(), b2verts.end() );

	b2ChainShape shape;
	{
		shape.CreateLoop( b2verts.data(), static_cast<int>( b2verts.size() ) );
	}

	b2FixtureDef fixture;
	{
		fixture.shape = &shape;
		fixture.density = 1.0f;
		fixture.friction = 0.3f;
		fixture.filter.categoryBits = static_cast<uint16>( parent_entity->collision_layer );
		fixture.filter.maskBits = static_cast<uint16>( parent_entity->collides_with );
		fixture.userData.pointer = id;
	}

	return body->CreateFixture( &fixture );
}

// ----------------------------------------------------------------------------

ec_b2d_static::ec_b2d_static( w_entity* parent_entity )
	: ec_b2d_body( parent_entity )
{
	type |= component_type::b2d_static;
	body_type = b2_staticBody;

	init_body();
}

// ----------------------------------------------------------------------------

ec_b2d_dynamic::ec_b2d_dynamic( w_entity* parent_entity )
	: ec_b2d_body( parent_entity )
{
	type |= component_type::b2d_dynamic;
	body_type = b2_dynamicBody;

	init_body();
}

// ----------------------------------------------------------------------------

ec_b2d_kinematic::ec_b2d_kinematic( w_entity* parent_entity )
	: ec_b2d_body( parent_entity )
{
	type |= component_type::b2d_kinematic;
	body_type = b2_kinematicBody;

	init_body();
}
