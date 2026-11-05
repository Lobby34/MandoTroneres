#pragma once

// DMX values that activate show, preshow, example show and home commands.
constexpr int DMX_COMMAND_THRESHOLD = 192;

// Movement command ranges; values retain the existing controller behavior.
constexpr int DMX_MOVEMENT_LOWER_BOUND = 128;
constexpr int DMX_MOVEMENT_UPPER_BOUND = 255;
constexpr int DMX_MOVEMENT_DEAD_LOW = 181;
constexpr int DMX_MOVEMENT_DEAD_HIGH = 199;

// Scheduler and shutter-button timing, in milliseconds.
constexpr unsigned long TIMELINE_INTERVAL_MS = 10;
constexpr unsigned long PERSIANA_BUTTON_PRESS_MS = 100;
