
#include "master_pch.h"
#include "master_header.h"

namespace war
{

float Slide_Def_Asset::get_left_slice_sz() const
{
	return patches[ slicedef_patch::top_left ]->rc.w;
}

float Slide_Def_Asset::get_right_slice_sz() const
{
	return patches[ slicedef_patch::top_right ]->rc.w;
}

float Slide_Def_Asset::get_top_slice_sz() const
{
	return patches[ slicedef_patch::top_left ]->rc.h;
}

float Slide_Def_Asset::get_bottom_slice_sz() const
{
	return patches[ slicedef_patch::bottom_left ]->rc.h;
}

}
