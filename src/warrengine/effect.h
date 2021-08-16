
namespace war
{

// ----------------------------------------------------------------------------

struct effect
{
	void set_life_cycle( e_life_cycle lc )
	{
		life_cycle.set( lc );
	}
	const life_cycle_mgr* get_life_cycle()
	{
		return &life_cycle;
	}

	virtual void update();

private:

	life_cycle_mgr life_cycle;
};

// ----------------------------------------------------------------------------

struct timed_effect : effect
{
	timed_effect( time_ms duration );

	// when this fx will be finished and should be deleted
	time_ms time_finish;

	virtual void update();
};

// ----------------------------------------------------------------------------

struct te_transform_shake : timed_effect
{
	te_transform_shake( time_ms duration, transform* tform, float strength );

	transform* tform;
	noise_simplex noise;
	float strength = 1.0f;

	virtual void update();
};


}
