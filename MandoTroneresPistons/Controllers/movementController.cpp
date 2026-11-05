#pragma once
#include <Conceptinetics.h>
#include "../configuration.h"
#include "../enviroment_dmx.h"
#include "../Classes/Head.cpp"

int readDmxCommand(DMX_Slave &dmx, int channel)
{
    if (DMX_SLAVE_ADDRESS < 1 || DMX_SLAVE_ADDRESS + DMX_SLAVE_CHANNELS - 1 > 512 ||
        channel < 1 || channel > DMX_SLAVE_CHANNELS)
    {
        return 0;
    }
    return dmx.getChannelValue(channel);
}

// State channel: 0 = no command, 1..127 = close, 128..255 = open.
// State commands override auto. A held auto signal repeats completed cycles.
void checkPistonCommand(DMX_Slave &dmx, PistoNeumatic &piston, int stateChannel, int autoChannel)
{
    const int state = readDmxCommand(dmx, stateChannel);
    if (state > 0)
    {
        if (state >= DMX_PISTON_OPEN_THRESHOLD)
        {
            piston.setState(PistonState::Open);
        }
        else
        {
            piston.setState(PistonState::Closed);
        }
    }
    else if (readDmxCommand(dmx, autoChannel) > DMX_COMMAND_THRESHOLD)
    {
        piston.startAutoMovement();
    }
    // No command leaves the state unchanged and lets an active cycle finish.
}

// 0 = no command; 1..85 = left; 86..170 = middle; 171..255 = right.
// Returns true when the head owns its two pistons for this loop pass.
bool checkHeadCommand(DMX_Slave &dmx, Head &head, int stateChannel)
{
    const int state = readDmxCommand(dmx, stateChannel);
    if (state == 0)
    {
        return false;
    }

    if (state <= DMX_HEAD_LEFT_MAX)
    {
        head.setState(HeadState::Left);
    }
    else if (state <= DMX_HEAD_MIDDLE_MAX)
    {
        head.setState(HeadState::Center);
    }
    else
    {
        head.setState(HeadState::Right);
    }
    return true;
}
