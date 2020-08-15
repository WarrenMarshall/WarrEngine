#pragma once

// ----------------------------------------------------------------------------

struct GE_API w_entity : i_lifecycle, i_transform
{
	std::vector<std::shared_ptr<w_entity_component>> components;

	void set_life_cycle( e_lifecycle lifecycle ) override;
	virtual bool can_be_deleted();

	// movement direction and speed
	w_vec2 dir;
	float speed = 1.0f;

	virtual void update();
	virtual void draw_components();
	virtual void post_spawn();

	template<typename T> T* add_component()
	{
		components.emplace_back( std::make_shared<T>() );
		return static_cast<T*>( components.back().get() );
	}
};

// ----------------------------------------------------------------------------

// a cozy is an entity that lives for X ms and then marks itself as dying
// so it'll get cleaned up.
//
// this is a handy way to spawn temp effects like explosions or muzzle flashes.

struct GE_API w_entity_cozy : w_entity
{
	float life_remaining = 0.0f;

	w_entity_cozy();

	void update() override;
};
