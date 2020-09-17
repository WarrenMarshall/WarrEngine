
#include "app_header.h"

layer_default::layer_default()
{
	draws_completely_solid = true;
}

void layer_default::push()
{
	world_geo = add_entity<w_entity>();
	world_geo->set_transform( { 75.0f, 150.0f }, 0, 1 );
	world_geo->add_component<ec_sprite>()->init( "engine_white_solid" )->scale = 0.5f;
	world_geo->add_component<ec_b2d_static>()->init_as_box( 50.0f, 5 );

	// dynamic
	{
		b2BodyDef body_definition;
		body_definition.type = b2_dynamicBody;
		body_definition.position.Set( 32.0f, 4.0f );
		dynamic_body = engine->box2d_world->CreateBody( &body_definition );

		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox( 1.0f, 1.0f );

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;

		dynamic_body->CreateFixture( &fixtureDef );
	}
}

void layer_default::update()
{
	w_layer::update();

	b2Vec2 position = dynamic_body->GetPosition();
	float angle = dynamic_body->GetAngle();

	log_msg( "Pos : {}, {} / Angle: {}", position.x, position.y, angle );
}

void layer_default::draw()
{
	w_layer::draw();
}
