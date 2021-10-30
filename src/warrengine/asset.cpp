
#include "master_pch.h"

namespace war
{

// the asset uses whatever resources it needs and creates it's internals. this
// can be anything from loading data from a file or creating structures from
// values in memory. whatever the assets needs to turn itself into something the
// engine can use.

bool Asset::create()
{
	return false;
}

// whenever the time dilation in the engine is changed, every asset is given the
// chance to do something in response. this is primarily for sound assets to
// adjust their pitch and playback but is open to any asset.

void Asset::adjust_for_time_dilation()
{
}

}
