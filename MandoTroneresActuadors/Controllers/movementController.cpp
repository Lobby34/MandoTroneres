#include <Conceptinetics.h>
#include "../configuration.h"

void checkMovementCommand(DMX_Slave &dmx_slave, MotorLineal &motor, int manualChannel, int manualFixedChannel, int autoChannel)
{
  int m = dmx_slave.getChannelValue(manualChannel);
  int mf = dmx_slave.getChannelValue(manualFixedChannel);
  int a = dmx_slave.getChannelValue(autoChannel);

  // PRIORITY CHECKING
  if (m > DMX_MOVEMENT_LOWER_BOUND)
  {

    // ----- 1. MANUAL MOVEMENT -----
    if (m >= DMX_MOVEMENT_LOWER_BOUND && m < DMX_MOVEMENT_DEAD_LOW)
    {
      motor.startDownMovement();
      return; // manual overrides everything else
    }
    else if (m > DMX_MOVEMENT_DEAD_HIGH && m <= DMX_MOVEMENT_UPPER_BOUND)
    {
      motor.startUpMovement();
      return; // manual overrides everything else
    }
    else if (m >= DMX_MOVEMENT_DEAD_LOW && m <= DMX_MOVEMENT_DEAD_HIGH)
    {
      motor.stopMovement(); // dead zone, do nothing
      return;
    }
  }
  else if (mf > DMX_MOVEMENT_LOWER_BOUND)
  {

    // ----- 2. MANUAL FIXED -----
    if (mf >= DMX_MOVEMENT_LOWER_BOUND && mf < DMX_MOVEMENT_DEAD_LOW)
    {
      motor.goDownMax();
      return; // manual-fixed down
    }
    else if (mf > DMX_MOVEMENT_DEAD_HIGH && mf <= DMX_MOVEMENT_UPPER_BOUND)
    {
      motor.goUpMax();
      return; // manual-fixed up
    }
    else if (mf >= DMX_MOVEMENT_DEAD_LOW && mf <= DMX_MOVEMENT_DEAD_HIGH)
    {
      return;
    }
  }
  else
  {

    // ----- 3. AUTO MODE -----
    if (a > DMX_MOVEMENT_LOWER_BOUND)
    {
      motor.startAutoMovement();
      return;
    }
  }

  // ----- 4. DEFAULT -----
  if (!motor.getLatched()) {
    motor.stopManualMovement();
  }
}

void checkMovementCommandInverted(DMX_Slave &dmx_slave, MotorLineal &motor, int manualChannel, int manualFixedChannel, int autoChannel)
{
  int m = dmx_slave.getChannelValue(manualChannel);
  int mf = dmx_slave.getChannelValue(manualFixedChannel);
  int a = dmx_slave.getChannelValue(autoChannel);

  // PRIORITY CHECKING
  if (m > DMX_MOVEMENT_LOWER_BOUND)
  {

    // ----- 1. MANUAL MOVEMENT -----
    if (m >= DMX_MOVEMENT_LOWER_BOUND && m < DMX_MOVEMENT_DEAD_LOW)
    {
      motor.startUpMovement();
      return; // manual overrides everything else
    }
    else if (m > DMX_MOVEMENT_DEAD_HIGH && m <= DMX_MOVEMENT_UPPER_BOUND)
    {
      motor.startDownMovement();
      return; // manual overrides everything else
    }
    else if (m >= DMX_MOVEMENT_DEAD_LOW && m <= DMX_MOVEMENT_DEAD_HIGH)
    {
      motor.stopMovement(); // dead zone, do nothing
      return;
    }
  }
  else if (mf > DMX_MOVEMENT_LOWER_BOUND)
  {

    // ----- 2. MANUAL FIXED -----
    if (mf >= DMX_MOVEMENT_LOWER_BOUND && mf < DMX_MOVEMENT_DEAD_LOW)
    {
      motor.goUpMax();
      return; // manual-fixed down
    }
    else if (mf > DMX_MOVEMENT_DEAD_HIGH && mf <= DMX_MOVEMENT_UPPER_BOUND)
    {
      motor.goDownMax();
      return; // manual-fixed up
    }
    else if (mf >= DMX_MOVEMENT_DEAD_LOW && mf <= DMX_MOVEMENT_DEAD_HIGH)
    {
      return;
    }
  }
  else
  {

    // ----- 3. AUTO MODE -----
    if (a > DMX_MOVEMENT_LOWER_BOUND)
    {
      motor.startAutoMovement();
      return;
    }
  }

  // ----- 4. DEFAULT -----
  if (!motor.getLatched()) {
    motor.stopManualMovement();
  }
}