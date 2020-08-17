
#include "master_pch.h"
#include "master_header.h"

bool g_allow_hot_reload;

std::unique_ptr<w_logfile> logfile;
std::unique_ptr<w_engine> engine;
w_game* game = nullptr;
