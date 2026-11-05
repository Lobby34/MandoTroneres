#pragma once

#define DMX_COMMAND_THRESHOLD 192
#define DMX_PISTON_OPEN_THRESHOLD 128
#define DMX_HEAD_LEFT_MAX 85
#define DMX_HEAD_MIDDLE_MAX 170
#define TIMELINE_INTERVAL_MS 10UL

// Milliseconds. Set both times before using a piston's auto movement.
// Open time is the delay between opening and closing (include travel/hold time).
// Close time is the delay after closing before another cycle may start.
// Zero disables auto movement until timings have been configured.
#define PISTON_1_OPEN_MS 5000UL
#define PISTON_1_CLOSE_MS 5000UL

#define PISTON_2_OPEN_MS 5000UL
#define PISTON_2_CLOSE_MS 5000UL

#define PISTON_3_OPEN_MS 5000UL
#define PISTON_3_CLOSE_MS 5000UL

#define PISTON_4_OPEN_MS 5000UL
#define PISTON_4_CLOSE_MS 5000UL

#define PISTON_5_OPEN_MS 5000UL
#define PISTON_5_CLOSE_MS 5000UL

#define PISTON_6_OPEN_MS 5000UL
#define PISTON_6_CLOSE_MS 5000UL
