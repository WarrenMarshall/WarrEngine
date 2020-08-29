
#include "master_pch.h"
#include "master_header.h"

float v_window_w = 320;
float v_window_h = 240;

std::unique_ptr<w_logfile> logfile;
std::unique_ptr<w_engine> engine;
w_game* game = nullptr;
