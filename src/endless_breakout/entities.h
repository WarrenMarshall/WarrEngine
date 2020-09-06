#pragma once

// ----------------------------------------------------------------------------

struct e_paddle : w_entity
{
	e_paddle();
};

// ----------------------------------------------------------------------------

struct powerup
{
	std::unique_ptr<w_timer> timer = nullptr;
	bool is_active = false;

	powerup();

	virtual void activate( w_entity* owner );
	virtual void deactivate( w_entity* owner );
	virtual void update();
};

struct pu_fireball : powerup
{
	std::vector<w_entity_component*> clean_up;

	virtual void activate( w_entity* owner ) final;
	virtual void deactivate( w_entity* owner ) final;
};

// ----------------------------------------------------------------------------

struct e_ball : w_entity
{
	e_ball();

	virtual void update() final;

	pu_fireball fireball_powerup;

	virtual void collided_with( w_entity* entity_hit, c2Manifold& hit ) final;
};

// ----------------------------------------------------------------------------

struct e_brick : w_entity
{
	e_brick();

	void collided_with( w_entity* entity_hit, c2Manifold& hit );
};

// ----------------------------------------------------------------------------

struct e_brick_multiball : w_entity
{
	e_brick_multiball();
};
// ----------------------------------------------------------------------------

struct e_brick_fireball : w_entity
{
	e_brick_fireball();
};
