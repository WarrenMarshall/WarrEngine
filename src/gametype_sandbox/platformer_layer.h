#pragma once

struct platformer_layer : w_layer
{
	std::unique_ptr<w_platformer_physics> plat_physics = nullptr;

	platformer_layer();

	tweeny::tween<float> test_tween;

	w_entity* player = nullptr;
	w_entity* mover = nullptr;
	tweeny::tween<float> tween_mover;

	virtual void push() override;
	virtual void pop() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool handle_input_event( const w_input_event* evt );

	void spawn_coins();
};
