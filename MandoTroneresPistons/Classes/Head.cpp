#pragma once
#include "pistoNeumatic.cpp"

enum HeadState : uint8_t { Left, Center, Right };

class Head
{
private:
    PistoNeumatic &first;
    PistoNeumatic &second;

public:
    // CONSTRUCTOR
    Head(PistoNeumatic &firstPiston, PistoNeumatic &secondPiston)
        : first(firstPiston), second(secondPiston)
    {
    }

    /** SETTERS AND GETTERS **/

    void setState(HeadState state)
    {
        // Both commands happen in the same loop pass, without waiting between them.
        if (state == HeadState::Left)
        {
            first.setState(PistonState::Closed);
            second.setState(PistonState::Closed);
        }
        else if (state == HeadState::Center)
        {
            first.setState(PistonState::Open);
            second.setState(PistonState::Closed);
        }
        else
        {
            first.setState(PistonState::Open);
            second.setState(PistonState::Open);
        }
    }

    HeadState getState() const
    {
        const bool firstOpen = first.getState() == PistonState::Open;
        const bool secondOpen = second.getState() == PistonState::Open;
        if (firstOpen && secondOpen)
        {
            return HeadState::Right;
        }
        if (!firstOpen && !secondOpen)
        {
            return HeadState::Left;
        }
        return HeadState::Center;
    }
};
