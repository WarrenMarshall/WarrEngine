#pragma once

struct layer_default : w_layer
{
	layer_default();

	w_entity* dynamic_body;

	b2BodyDef groundBodyDef;
	b2PolygonShape groundBox;

	b2BodyDef bodyDef;
	b2Body* body;

	virtual void push() override;
	virtual void update() override;
	virtual void draw() override;
};
