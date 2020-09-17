
#include "app_header.h"

layer_default::layer_default()
{
	draws_completely_solid = true;
}

void layer_default::push()
{
	dynamic_body = add_entity<w_entity>();
	dynamic_body->add_component<ec_collider>()->init_as_box( w_rect( -5, -5, 5, 5 ) );
	dynamic_body->debug_draw_collision = true;

	groundBodyDef.position.Set( 0.0f, 100.0f );
	b2Body* groundBody = engine->box2d_world->CreateBody( &groundBodyDef );
	groundBox.SetAsBox( 50.0f, 10.0f );
	groundBody->CreateFixture( &groundBox, 0.0f );

	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set( 32.0f, 4.0f );
	body = engine->box2d_world->CreateBody( &bodyDef );

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox( 1.0f, 1.0f );

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	body->CreateFixture( &fixtureDef );
}

void layer_default::update()
{
	w_layer::update();

	b2Vec2 position = body->GetPosition();
	float angle = body->GetAngle();

	dynamic_body->set_pos( { position.x, position.y } );
	dynamic_body->angle_facing = angle;

	//log_msg( "Pos : {}, {} / Angle : {}", position.x, position.y, angle );
}

void layer_default::draw()
{
	w_layer::draw();
}
