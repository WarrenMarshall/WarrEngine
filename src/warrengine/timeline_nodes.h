
#pragma once

namespace war
{

struct timeline_nodes
{
	time_ms start, end;
	float duration;

	std::vector<std::unique_ptr<timeline_nodes_key_frame>> key_frames;
	e_timeline_type type = timeline_type::invalid;

	timeline_nodes();

	void clear();
	void init( time_ms duration );
	void update();

	// #cleanup - if these are going to do nothing, just remove them and make life_cycle public
	void set_life_cycle( e_life_cycle lc ) { life_cycle.set( lc ); }
	const life_cycle_mgr* get_life_cycle() { return &life_cycle; }

	template<typename T, typename ...Args>
	timeline_nodes_key_frame* add_key_frame( Args &&...args )
	{
		key_frames.push_back( std::make_unique<T>( std::forward<Args>( args )... ) );
		return key_frames.back().get();
	}

private:
	life_cycle_mgr life_cycle;
};

}
