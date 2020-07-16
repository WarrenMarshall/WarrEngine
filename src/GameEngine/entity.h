#pragma once

// ----------------------------------------------------------------------------

struct w_entity : i_lifecycle, i_transform
{
	std::vector<std::unique_ptr<w_entity_component>> components;

	void set_life_cycle( e_lifecycle lifecycle ) override;
	virtual bool can_be_deleted();

#ifdef _DEBUG
	// if true, this entity will draw it's bounds every frame
	bool draw_bounds = false;
#endif

	// movement direction and speed
	w_vec2 dir;
	float speed = 1.0f;

	// collision bounds
	std::unique_ptr<w_bounds> bounds = nullptr;

	w_entity();
	~w_entity();

	virtual bool bounds_contains_point( w_vec2 pos_to_check );
	virtual void update();
	virtual void draw_components();
	virtual void draw();
	virtual void post_spawn();

	template<typename T> T* add_component()
	{
		components.emplace_back( std::make_unique<T>() );

		T* new_component = static_cast<T*>( components.back().get() );
		return new_component;
	}
};

// ----------------------------------------------------------------------------

// a cozy is an entity that lives for X ms and then marks itself as dying
// so it'll get cleaned up.
//
// this is a handy way to spawn temp effects like explosions or muzzle flashes.

struct w_entity_cozy : w_entity
{
	float life_remaining = 0.0f;

	w_entity_cozy();
	~w_entity_cozy();

	virtual void update();
};
