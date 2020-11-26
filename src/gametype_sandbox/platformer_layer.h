#pragma once

struct w_mover_controller
{
	float start, end, time_ms;
	std::unique_ptr<w_tween> tween = nullptr;

	// time_ms = how long it takes to move from "start" to "end"

	w_mover_controller( float start, float end, float time_ms )
	{
		this->start = start;
		this->end = end;
		this->time_ms = time_ms;

		tween = std::make_unique<w_tween>( tween_type::pingpong, -1.0f, 1.0f, time_ms );
	}

	void update()
	{
		tween->update();
	}
};


struct platformer_layer : w_layer
{
	std::unique_ptr<w_platformer_contact_listener> plat_physics = nullptr;

	platformer_layer();

	w_entity* player = nullptr;
	w_entity* mover = nullptr;
	std::unique_ptr<w_mover_controller> mover_controller = nullptr;

	virtual void push() override;
	virtual void pop() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool event_input_pressed( const w_input_event* evt ) override;

	void spawn_coins();
};
