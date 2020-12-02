
#pragma once

#define game (static_cast<template_platformer*>(base_game.get()))

#include <warr_engine.h>

// ----------------------------------------------------------------------------

constexpr float player_move_force = 15;
constexpr float player_move_force_max = 100;
constexpr float player_jump_force = 325;

constexpr float player_base_radius = 6;
constexpr float player_drop_down_normal_tolerance = 0.8f;
constexpr int player_jump_interval = 75;
constexpr float player_air_control_damping = 1.0f;

// ----------------------------------------------------------------------------

static w_bit_flag_generator clayer_bits(2);

static unsigned clayer_world = clayer_bits.next();
static unsigned clayer_player = clayer_bits.next();
static unsigned clayer_coin = clayer_bits.next();

// ----------------------------------------------------------------------------
// forward declarations

struct w_player_input_receiver;
struct w_platformer_physic_responder;

// ----------------------------------------------------------------------------

#include "layer_main_menu.h"
#include "template_platformer.h"

#include "entities.h"
#include "physics.h"
#include "player_input_receiver.h"
#include "layer_game.h"
