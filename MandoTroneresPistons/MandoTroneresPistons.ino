#define _TASK_LTS_POINTER // Enables LTS for v4.0.3
#include <TaskScheduler.h>
#include <Conceptinetics.h>
#include "configuration.h"
#include "enviroment_pins.h"
#include "enviroment_dmx.h"

// Construct the scheduler before the pistons register their tasks.
Scheduler runner;
#include "Classes/pistoNeumatic.cpp"
#include "Classes/Head.cpp"
#include "Controllers/movementController.cpp"

DMX_Slave dmx_slave(DMX_SLAVE_CHANNELS, RXEN_PIN);
boolean showActive = false;
unsigned long showStartTime = 0;
int timelineIndex = 0;

struct TimelineEvent
{
    unsigned long timeMs;
    void (*action)();
};
TimelineEvent *currentTimeline = nullptr;
int currentTimelineLength = 0;

PistoNeumatic head1(PISTON_1_PIN, PISTON_1_OPEN_MS, PISTON_1_CLOSE_MS);
PistoNeumatic head2(PISTON_2_PIN, PISTON_2_OPEN_MS, PISTON_2_CLOSE_MS);
PistoNeumatic dorsal(PISTON_3_PIN, PISTON_3_OPEN_MS, PISTON_3_CLOSE_MS);
PistoNeumatic leftFin(PISTON_4_PIN, PISTON_4_OPEN_MS, PISTON_4_CLOSE_MS);
PistoNeumatic rightFin(PISTON_5_PIN, PISTON_5_OPEN_MS, PISTON_5_CLOSE_MS);
PistoNeumatic piston6(PISTON_6_PIN, PISTON_6_OPEN_MS, PISTON_6_CLOSE_MS);

// Optional head pairings. Edit these to match the mechanism.
Head head(head1, head2);

#include "Shows/show.h"
#include "Shows/preshow.h"
#include "Shows/exampleShow.h"

Task tTimeline(
    TIMELINE_INTERVAL_MS, TASK_FOREVER, []
    {
        const unsigned long elapsed = millis() - showStartTime;
        while (timelineIndex < currentTimelineLength && currentTimeline[timelineIndex].timeMs <= elapsed)
        {
            currentTimeline[timelineIndex].action();
            timelineIndex++;
        }
        if (timelineIndex >= currentTimelineLength)
        {
            tTimeline.disable();
            showActive = false;
        }
    }, &runner, false);

void startTimeline(TimelineEvent *timeline, int length)
{
    currentTimeline = timeline;
    currentTimelineLength = length;
    timelineIndex = 0;
    showStartTime = millis();
    showActive = true;
    tTimeline.enable();
}

void setup()
{
#if DEBUG_HEARTBEAT
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
#endif
    head1.begin();
    head2.begin();
    dorsal.begin();
    leftFin.begin();
    rightFin.begin();
    piston6.begin();

    if (DMX_SLAVE_ADDRESS >= 1 && DMX_SLAVE_ADDRESS + DMX_SLAVE_CHANNELS - 1 <= 512)
    {
        dmx_slave.setStartAddress(DMX_SLAVE_ADDRESS);
        dmx_slave.enable();
    }
    runner.startNow();
}

void loop()
{
#if DEBUG_HEARTBEAT
    static unsigned long lastHeartbeat = 0;
    static bool heartbeatOn = false;
    if (millis() - lastHeartbeat >= 500UL)
    {
        lastHeartbeat = millis();
        heartbeatOn = !heartbeatOn;
        digitalWrite(LED_BUILTIN, heartbeatOn ? HIGH : LOW);
    }
#endif
    runner.execute();
    if (tTimeline.isEnabled())
    {
        return;
    }

    if (readDmxCommand(dmx_slave, DMX_SHOW_CHANNEL) > DMX_COMMAND_THRESHOLD)
    {
        startTimeline(mainShow, sizeof(mainShow) / sizeof(mainShow[0]));
    }
    else if (readDmxCommand(dmx_slave, DMX_PRESHOW_CHANNEL) > DMX_COMMAND_THRESHOLD)
    {
        runPreshow();
    }
    else if (readDmxCommand(dmx_slave, DMX_EXAMPLESHOW_CHANNEL) > DMX_COMMAND_THRESHOLD)
    {
        startTimeline(exampleShow, sizeof(exampleShow) / sizeof(exampleShow[0]));
    }
    else
    {
        // A head command takes priority over direct commands for its piston pair.
        if (!checkHeadCommand(dmx_slave, head, DMX_HEAD_1_STATE_CHANNEL))
        {
            checkPistonCommand(dmx_slave, head1, DMX_PISTON_1_STATE_CHANNEL, DMX_PISTON_1_AUTO_CHANNEL);
            checkPistonCommand(dmx_slave, head2, DMX_PISTON_2_STATE_CHANNEL, DMX_PISTON_2_AUTO_CHANNEL);
        }
        checkPistonCommand(dmx_slave, dorsal, DMX_PISTON_3_STATE_CHANNEL, DMX_PISTON_3_AUTO_CHANNEL);
        checkPistonCommand(dmx_slave, leftFin, DMX_PISTON_4_STATE_CHANNEL, DMX_PISTON_4_AUTO_CHANNEL);
        checkPistonCommand(dmx_slave, rightFin, DMX_PISTON_5_STATE_CHANNEL, DMX_PISTON_5_AUTO_CHANNEL);
        checkPistonCommand(dmx_slave, piston6, DMX_PISTON_6_STATE_CHANNEL, DMX_PISTON_6_AUTO_CHANNEL);
    }
}
