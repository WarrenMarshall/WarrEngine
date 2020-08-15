
#include "master_pch.h"
#include "master_header.h"

bool GE_API g_allow_hot_reload;

std::unique_ptr<w_logfile> GE_API logfile;
std::unique_ptr<w_engine> GE_API engine;
GE_API w_game* game = nullptr;
