#define _TASK_LTS_POINTER // Enables LTS for v4.0.3

// Library improts
#include <TaskScheduler.h>  //Task scheduler so we can call "asyinc" functions
#include <Conceptinetics.h> //For dmx comunication

// File imports
#include "configuration.h"
#include "./Classes/motorLineal.cpp" //Import Classes
#include "./enviroment_pins.h"       //Import Declaration of enviroment variables
#include "./enviroment_dmx.h"
#include "./Classes/motorPersiana.cpp"
#include "./Controllers/movementController.cpp"
// #include "./Controllers/taskController.h"
// #include "./Controllers/taskController.cpp"

// DMX Startup
DMX_Slave dmx_slave(DMX_SLAVE_CHANNELS);

// Variable Delaration
Scheduler runner;                // Global – used by motorLineal
boolean showActive = false;      // Global - used to know if show is active
int timelineIndex = 0;           // Global - to know the current position in the timeline
unsigned long showStartTime = 0; // Global - to know when the show started

// Selected Show Timeline Event
struct TimelineEvent
{
  unsigned long timeMs;
  void (*action)();
};

TimelineEvent *currentTimeline = nullptr; // Pointer to current timeline
int currentTimelineLength = 0;            // Length of current timeline

// Object declaration
MotorLineal mouth(MOTOR_1_A_PIN, MOTOR_1_B_PIN, MOTOR_1_MAX_MILLIS_OPENED);
MotorLineal lighthouse(MOTOR_2_A_PIN, MOTOR_2_B_PIN, MOTOR_2_MAX_MILLIS_OPENED);

// Show Timeline
TimelineEvent mainShow[] = {
    {0, [] { showActive = true; }},
    {1000, [] {}},
    {2000, [] { showActive = false; }}
};

TimelineEvent exampleShow[] = {
    {0, []{showActive = true;}},
    {2500,   [] {}},
    {4500,  [] { showActive = false; }}};

// Task declaration to run the showTimeline
Task tTimeline(
    TIMELINE_INTERVAL_MS, -1, []
    {
    unsigned long elapsed = millis() - showStartTime;

    while (timelineIndex < currentTimelineLength && currentTimeline[timelineIndex].timeMs <= elapsed) {
      currentTimeline[timelineIndex].action();
      timelineIndex++;
    }

    if (timelineIndex >= currentTimelineLength) {
      tTimeline.disable();
      showActive = false;
    } },
    &runner, false);

void startTimeline(TimelineEvent *timelineArray, int length)
{
  currentTimeline = timelineArray;
  currentTimelineLength = length;
  timelineIndex = 0;
  showStartTime = millis();
  showActive = true;
  tTimeline.enable();
}

void setup()
{
  // DMX SETUP
  dmx_slave.enable();
  dmx_slave.setStartAddress(DMX_SLAVE_ADDRESS);

  // Inicialize task Scheduler
  runner.startNow();
}

void loop()
{
  // Task scheduler "move one step"
  runner.execute();

  if (!tTimeline.isEnabled())
  {
    // SHOW
    if (dmx_slave.getChannelValue(DMX_SHOW_CHANNEL) > DMX_COMMAND_THRESHOLD)
    {
      startTimeline(mainShow, sizeof(mainShow) / sizeof(mainShow[0]));
    }
    // PRESHOW
    else if (dmx_slave.getChannelValue(DMX_PRESHOW_CHANNEL) > DMX_COMMAND_THRESHOLD)
    {
      mouth.goDownMax();
    }
    // EXAMPLE SHOW
    else if (dmx_slave.getChannelValue(DMX_EXAMPLESHOW_CHANNEL) > DMX_COMMAND_THRESHOLD)
    {
      startTimeline(exampleShow, sizeof(exampleShow) / sizeof(exampleShow[0]));
    }
    else
    {
      // SET CURRENT POSITION AS HOME
      if (dmx_slave.getChannelValue(DMX_SETHOMEPOSITIONS_CHANNEL) > DMX_COMMAND_THRESHOLD)
      {
        mouth.setMillisOpened(0);
      }

      // CHECK ALL MOVEMENT COMMANDS OF ALL MOTORS
      checkMovementCommand(dmx_slave, mouth, DMX_MOUTH_MANUAL_CHANNEL, DMX_MOUTH_MANUALFIXED_CHANNEL, DMX_MOUTH_AUTO_CHANNEL);
      checkMovementCommand(dmx_slave, lighthouse, DMX_LIGHTHOUSE_MANUAL_CHANNEL, DMX_LIGHTHOUSE_MANUALFIXED_CHANNEL, DMX_LIGHTHOUSE_AUTO_CHANNEL);
    }
  }
}