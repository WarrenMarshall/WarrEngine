
#include "master_pch.h"
#include "master_header.h"

// adds a listener to this speaker

void i_speaker::add_listener( i_listener* listener )
{
	// if this listener is already attached, leave so we don't get duplicates
	auto find_iter = std::find( listeners.begin(), listeners.end(), listener );
	if( find_iter != listeners.end() )
	{
		return;
	}

	listeners.emplace_back( listener );
}

// removes a listener from this speaker

void i_speaker::remove_listener( i_listener* listener )
{
	auto find_iter = std::find( listeners.begin(), listeners.end(), listener );
	if( find_iter == listeners.end() )
		log_error( fmt::format( "{} : listener not found!", __FUNCTION__ ) );

	listeners.erase( find_iter );
}

// sends an event to all listeners

void i_speaker::send_event_to_listeners( e_event_id event, void* object )
{
	for( const auto& iter : listeners )
	{
		iter->on_listener_event_received( event, object );
	}
}