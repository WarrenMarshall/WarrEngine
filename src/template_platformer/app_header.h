
#pragma once

#define game (static_cast<template_platformer*>(base_game.get()))

#include <warr_engine.h>

// ----------------------------------------------------------------------------

static w_bit_flag_generator clayer_bits(2);

static unsigned clayer_world = clayer_bits.next();
static unsigned clayer_player = clayer_bits.next();
static unsigned clayer_coin = clayer_bits.next();

// ----------------------------------------------------------------------------

#include "layer_main_menu.h"
#include "template_platformer.h"

#include "entities.h"
#include "physics.h"
#include "layer_game.h"
