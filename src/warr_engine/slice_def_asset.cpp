
#include "master_pch.h"
#include "master_header.h"

namespace war
{

float slice_def_asset::get_left_slice_sz() const
{
	return patches[ slicedef_patch::top_left ]->rc.w;
}

float slice_def_asset::get_right_slice_sz() const
{
	return patches[ slicedef_patch::top_right ]->rc.w;
}

float slice_def_asset::get_top_slice_sz() const
{
	return patches[ slicedef_patch::top_left ]->rc.h;
}

float slice_def_asset::get_bottom_slice_sz() const
{
	return patches[ slicedef_patch::bottom_left ]->rc.h;
}

}
