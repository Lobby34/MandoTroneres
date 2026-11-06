# MandoTroneres

Arduino controllers for the Troneres animatronics, operated through DMX or timed show sequences.

This repository contains two separate sketches:

- **MandoTroneresActuadors** controls linear actuators.
- **MandoTroneresPistons** controls pneumatic pistons and a head with left, center and right positions.

## Getting started

1. Open the `.ino` inside the controller folder you want to use in Arduino IDE.
2. Install **TaskScheduler 4.0.3** and **Conceptinetics**.
3. Select your board and port.
4. Check the pin assignments, DMX settings and movement timings below, then compile and upload.

Each controller is uploaded independently to its own Arduino.

## Configuration

Inside the controller's folder:

| File | What to change |
| --- | --- |
| `enviroment_pins.h` | Pins connected to the hardware; actuator travel limits also live here. |
| `enviroment_dmx.h` | DMX start address and channel assignments. Channels are relative to the start address, beginning at 1. |
| `configuration.h` | Movement timings and command thresholds. Times are in milliseconds. |

For the piston controller, use `-1` for unused pins or channels. Automatic movement opens a piston, waits its open time, closes it, then waits its close time. Holding the DMX auto command repeats this cycle.

## Editing shows

Each controller has a `Shows` folder:

- **show.h**: the main show timeline.
- **preshow.h**: positions to prepare before the show.
- **exampleShow.h**: a separate timeline for trying out movements.

Timeline entries pair a time since the show started with an action. For example, in the piston controller:

```cpp
{1000, [] { head.setState(Center); }}, // Center the head after one second
{3000, [] { dorsal.setState(Open); }}, // Open the dorsal piston after three seconds
```

Shows can use objects declared before the show includes in the main `.ino`. Keep those declarations above the includes when adding new movements.

## Development

Work on **main**. The **2026** branch preserves the previous final version.
