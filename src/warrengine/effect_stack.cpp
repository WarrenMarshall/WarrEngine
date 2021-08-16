
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void effect_stack::update()
{
	for( auto& e : stack )
	{
		e->update();
	}

	// remove dead effects

	for( auto idx = 0 ; idx < stack.size() ; ++idx )
	{
		if( stack[idx].get()->get_life_cycle()->is_dying() )
		{
			stack.erase( stack.begin() + idx );
			idx--;
		}
	}
}

}
