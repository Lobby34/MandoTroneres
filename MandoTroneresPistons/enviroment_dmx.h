#pragma once

// -1 means unassigned. DMX input stays disabled until a valid start address is set.
#define DMX_SLAVE_ADDRESS 1
#define DMX_SLAVE_CHANNELS 30 // Receive capacity, not a channel assignment.

#define DMX_SHOW_CHANNEL 1
#define DMX_PRESHOW_CHANNEL 2
#define DMX_EXAMPLESHOW_CHANNEL 3

// Channels are relative to the start address, starting at 1.
// Leave piston channels unassigned when controlling them through a head.
#define DMX_PISTON_1_STATE_CHANNEL 13
#define DMX_PISTON_1_AUTO_CHANNEL 15

#define DMX_PISTON_2_STATE_CHANNEL 14
#define DMX_PISTON_2_AUTO_CHANNEL 16

#define DMX_PISTON_3_STATE_CHANNEL 9
#define DMX_PISTON_3_AUTO_CHANNEL 5

#define DMX_PISTON_4_STATE_CHANNEL 10
#define DMX_PISTON_4_AUTO_CHANNEL 6

#define DMX_PISTON_5_STATE_CHANNEL 11
#define DMX_PISTON_5_AUTO_CHANNEL 7

#define DMX_PISTON_6_STATE_CHANNEL 12
#define DMX_PISTON_6_AUTO_CHANNEL 8

#define DMX_HEAD_1_STATE_CHANNEL 4
