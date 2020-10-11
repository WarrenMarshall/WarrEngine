\
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
	anim_offset = w_random::getf();
}

w_entity_component* ec_sprite::init( const std::string_view tex_name )
{
	tex = engine->get_asset<a_texture>( tex_name );
	return this;
}

void ec_sprite::draw()
{
	if( is_dead() )
	{
		return;
	}

	MATRIX
		->top()
		->scale( flip_x ? -1.0f : 1.0f, flip_y ? -1.0f : 1.0f );

	RENDER->draw_sprite( tex->get_subtexture( anim_offset ), pos );
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

	post_init();

	return this;
}

bool ec_emitter::is_fully_dead()
{
	if( w_entity_component::is_fully_dead() )
	{
		if( is_dying() && emitter->particle_pool->num_alive == 0 )
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

void ec_emitter::post_init()
{
	// particle warm ups require the parent and component transforms to be applied
	// so the warmed up particles spawn at the right position in the world.

	MATRIX
		->push()
		->add_transform( parent_entity->pos, parent_entity->angle, parent_entity->scale )
		->add_transform( pos, angle, scale );

	emitter->post_init();

	MATRIX->pop();
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

		engine->box2d_world->DestroyBody( body );
		body = nullptr;
	}
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

// rc - the top left of the box (relative to body) and the w/h
b2Fixture* ec_b2d_body::add_fixture_box( const char* id, w_rect rc )
{
	w_vec2 pos = { rc.x + ( rc.w / 2.0f ), rc.y + ( rc.h / 2.0f ) };

	return add_fixture_box( id, pos, rc.w, rc.h );
}

// pos - middle of box, relative to body
// w/h - size of box
b2Fixture* ec_b2d_body::add_fixture_box( const char* id, w_vec2 pos, float w, float h )
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
		fixture.restitution = 0.0f;
		fixture.filter.categoryBits = static_cast<uint16>( parent_entity->collision_layer );
		fixture.filter.maskBits = static_cast<uint16>( parent_entity->collides_with );
		fixture.userData.pointer = (uintptr_t) id;
	}

	return body->CreateFixture( &fixture );
}

b2Fixture* ec_b2d_body::add_fixture_circle( const char* id, w_vec2 pos, float radius )
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
		fixture.restitution = 0.0f;
		fixture.filter.categoryBits = static_cast<uint16>( parent_entity->collision_layer );
		fixture.filter.maskBits = static_cast<uint16>( parent_entity->collides_with );
		fixture.userData.pointer = (uintptr_t) id;
	}

	return body->CreateFixture( &fixture );
}

b2Fixture* ec_b2d_body::add_fixture_line( const char* id, w_vec2 pos, w_vec2 start, w_vec2 end )
{
	body->SetTransform( parent_entity->pos.to_b2d(), 0.0f );

	b2EdgeShape shape;
	{
		start += pos;
		end += pos;

		float length = ( end - start ).get_size_squared() / 10.0f;

		shape.SetOneSided(
			( start + w_vec2( -length, length ) ).to_b2d(),
			start.to_b2d(),
			end.to_b2d(),
			( end + w_vec2( length, length ) ).to_b2d()
		);
	}

	b2FixtureDef fixture;
	{
		fixture.shape = &shape;
		fixture.density = 1.0f;
		fixture.friction = 0.3f;
		fixture.restitution = 0.0f;
		fixture.filter.categoryBits = static_cast<uint16>( parent_entity->collision_layer );
		fixture.filter.maskBits = static_cast<uint16>( parent_entity->collides_with );
		fixture.userData.pointer = (uintptr_t) id;
	}

	return body->CreateFixture( &fixture );
}

b2Fixture* ec_b2d_body::add_fixture_line_loop( const char* id, w_vec2 pos, const std::vector<w_vec2>& verts )
{
	// Box2D requirement
	assert( verts.size() >= 3 );

	body->SetTransform( parent_entity->pos.to_b2d(), 0.0f );

	// convert the vertex list into a box2d friendly format
	std::vector<b2Vec2> b2verts;

	for( w_vec2 v : verts )
	{
		b2verts.push_back( ( v + pos ).to_b2d() );
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
		fixture.restitution = 0.0f;
		fixture.filter.categoryBits = static_cast<uint16>( parent_entity->collision_layer );
		fixture.filter.maskBits = static_cast<uint16>( parent_entity->collides_with );
		fixture.userData.pointer = (uintptr_t) id;
	}

	return body->CreateFixture( &fixture );
}

b2Fixture* ec_b2d_body::add_fixture_polygon( const char* id, w_vec2 pos, const std::vector<w_vec2>& verts )
{
	body->SetTransform( parent_entity->pos.to_b2d(), 0.0f );

	// convert the vertex list into a box2d friendly format
	std::vector<b2Vec2> b2verts;

	for( w_vec2 v : verts )
	{
		b2verts.push_back( ( v + pos ).to_b2d() );
	}

	b2PolygonShape shape;
	{
		shape.Set( b2verts.data(), static_cast<int>( b2verts.size() ) );
	}

	b2FixtureDef fixture;
	{
		fixture.shape = &shape;
		fixture.density = 1.0f;
		fixture.friction = 0.3f;
		fixture.restitution = 0.0f;
		fixture.filter.categoryBits = static_cast<uint16>( parent_entity->collision_layer );
		fixture.filter.maskBits = static_cast<uint16>( parent_entity->collides_with );
		fixture.userData.pointer = (uintptr_t) id;
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

// ----------------------------------------------------------------------------

ec_tilemap::ec_tilemap( w_entity* parent_entity )
	: w_entity_component( parent_entity )
{
	type |= component_type::tilemap;
}

w_entity_component* ec_tilemap::init()
{
	return this;
}

void ec_tilemap::draw()
{
}

void ec_tilemap::load_from_disk( const char* tag, std::string_view level_filename )
{
	auto b2d_static = parent_entity->get_component<ec_b2d_static>( component_type::b2d_body );

	auto file = engine->fs->load_file_into_memory( level_filename );
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_buffer( file->buffer->data(), std::size( *file->buffer.get() ) );

	if( result )
	{
		std::string key;

		// tilemap files only have one child - the map object
		pugi::xml_node map_node = doc.first_child();

		width = map_node.attribute( "width" ).as_int();
		height = map_node.attribute( "height" ).as_int();
		tilewidth = map_node.attribute( "tilewidth" ).as_int();
		tileheight = map_node.attribute( "tileheight" ).as_int();

		for( pugi::xml_node child : map_node.children() )
		{
			std::string type = child.name();

			// collision geometry
			if( type == "objectgroup" )
			{
				for( pugi::xml_node object : child.children() )
				{
					float x = snap_to_pixel( object.attribute( "x" ).as_float() );
					float y = snap_to_pixel( object.attribute( "y" ).as_float() );
					float w = snap_to_pixel( object.attribute( "width" ).as_float() );
					float h = snap_to_pixel( object.attribute( "height" ).as_float() );

					if( h == 0 )
					{
						// a height of zero indicates an edge segment
						b2d_static->add_fixture_line( tag, w_vec2::zero, { x, y }, { x + w, y + h } );
					}
					else
					{
						// anything with a positive height, we assume it's a box
						b2d_static->add_fixture_polygon( tag, w_vec2::zero,
														 {
														 { x, y },
														 { x + w, y },
														 { x + w, y + h },
														 { x, y + h }
														 }
						);
					}
				}
			}
		}
	}
}
