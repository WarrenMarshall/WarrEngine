
namespace war
{

// ----------------------------------------------------------------------------
// base of all the effects

struct effect
{
	effect( bool should_restore_state );

	void set_life_cycle( e_life_cycle lc )
	{
		life_cycle.set( lc );
	}
	const life_cycle_mgr* get_life_cycle()
	{
		return &life_cycle;
	}

	virtual void update();

protected:

	life_cycle_mgr life_cycle;
	bool should_restore_state : 1 = false;
};

// ----------------------------------------------------------------------------
// an effect that lasts for X milliseconds and dies

struct timed_effect : effect
{
	timed_effect( bool should_restore_state, time_ms duration );

	// when this fx will be finished and should be deleted
	time_ms time_finish;

	virtual void update();
};

// ----------------------------------------------------------------------------
// rumbles the angle

struct te_transform_shake_angle : timed_effect
{
	te_transform_shake_angle( bool should_restore_state, time_ms duration, transform* tform, float strength );

	struct
	{
		float angle;
	} saved_state;

	transform* tform;
	noise_simplex noise;
	float strength = 1.0f;

	virtual void update();
};

}
