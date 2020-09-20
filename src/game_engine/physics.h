#pragma once

// ----------------------------------------------------------------------------

struct w_raycast_callback_closest : b2RayCastCallback
{
	std::optional<b2Vec2> hit_point = std::nullopt;

	virtual float ReportFixture( b2Fixture* fixture, const b2Vec2& point,
								 const b2Vec2& normal, float fraction ) override;
};

// ----------------------------------------------------------------------------
