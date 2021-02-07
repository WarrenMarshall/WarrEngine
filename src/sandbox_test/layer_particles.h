#pragma once

// ----------------------------------------------------------------------------

struct layer_particles : w_layer
{
	layer_particles();

	virtual void push() override;
	virtual void update() override;
	virtual void draw() override;
};
