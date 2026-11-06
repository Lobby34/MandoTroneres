#pragma once
#include <Arduino.h>
#include <TaskScheduler.h>
#include "../configuration.h"

// Global scheduler (extern from main sketch)
extern Scheduler runner;

enum PistonState : uint8_t { Closed, Open };

class PistoNeumatic
{
private:
    PistonState state = PistonState::Closed;
    uint8_t pin;
    bool configured;
    unsigned long openTimeMs;
    unsigned long closeTimeMs;
    bool autoActive = false;
    Task tAuto;

    void writeState(PistonState selectedState)
    {
        state = selectedState;
        if (configured && PISTON_OUTPUTS_ENABLED)
        {
            if (state == PistonState::Open)
            {
                // Active-low output: LOW opens the piston.
                digitalWrite(pin, LOW);
            }
            else
            {
                digitalWrite(pin, HIGH);
            }
        }
    }

    // Static callback retrieves this piston through the task LTS pointer.
    static void autoCallback()
    {
        PistoNeumatic *self = static_cast<PistoNeumatic *>(runner.currentTask().getLtsPointer());
        if (!self)
        {
            return;
        }
        if (self->state == PistonState::Open)
        {
            self->writeState(PistonState::Closed);
            self->tAuto.setInterval(self->closeTimeMs);
            self->tAuto.restartDelayed();
        }
        else
        {
            self->autoActive = false;
        }
    }

public:
    // CONSTRUCTOR
    PistoNeumatic(int outputPin, unsigned long openMs, unsigned long closeMs)
        : pin(static_cast<uint8_t>(outputPin)), configured(outputPin >= 0 && outputPin < 255),
          openTimeMs(openMs), closeTimeMs(closeMs),
          tAuto(0, TASK_ONCE, &PistoNeumatic::autoCallback, &runner, false)
    {
        tAuto.setLtsPointer(static_cast<void *>(this));
    }

    // PISTON FUNCTIONS AND LOGIC

    // Called from setup, after the Arduino hardware has initialized.
    void begin()
    {
        if (configured && PISTON_OUTPUTS_ENABLED)
        {
            digitalWrite(pin, HIGH);
            pinMode(pin, OUTPUT);
        }
        setState(PistonState::Closed);
    }

    // Explicit commands cancel any unfinished automatic cycle.
    void setState(PistonState selectedState)
    {
        tAuto.disable();
        autoActive = false;
        writeState(selectedState);
    }

    void changeState()
    {
        if (state == PistonState::Open)
        {
            setState(PistonState::Closed);
        }
        else
        {
            setState(PistonState::Open);
        }
    }

    // One complete cycle. Repeated requests while busy do not restart its timer.
    void startAutoMovement()
    {
        if (!configured || autoActive || openTimeMs == 0 || closeTimeMs == 0)
        {
            return;
        }
        autoActive = true;
        writeState(PistonState::Open);
        tAuto.setInterval(openTimeMs);
        tAuto.restartDelayed();
    }

    /** SETTERS AND GETTERS **/

    // State is the commanded valve state, not sensor-confirmed physical position.
    PistonState getState() const
    {
        return state;
    }

    bool getAutoActive() const
    {
        return autoActive;
    }

    bool isConfigured() const
    {
        return configured;
    }

};
