
#include "master_pch.h"
#include "master_header.h"

implement_find_func( a_9slice_def )

float a_9slice_def::get_left_slice_sz()
{
	return patches[ slicedef_patch::P_00 ]->rc.w;
}

float a_9slice_def::get_right_slice_sz()
{
	return patches[ slicedef_patch::P_20 ]->rc.w;
}

float a_9slice_def::get_top_slice_sz()
{
	return patches[ slicedef_patch::P_00 ]->rc.h;
}

float a_9slice_def::get_bottom_slice_sz()
{
	return patches[ slicedef_patch::P_02 ]->rc.h;
}
