#pragma once


namespace war
{

struct collision_primitive
{

};

struct collision_primitive_aabb : collision_primitive
{
	c2AABB bbox;
};

}
