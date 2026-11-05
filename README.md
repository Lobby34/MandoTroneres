# MandoTroneres

Two independent Arduino sketches:

- `MandoTroneresActuadors/MandoTroneresActuadors.ino`: the existing actuator controller, with its show timelines and DMX behavior preserved.
- `MandoTroneresPistons/MandoTroneresPistons.ino`: the pneumatic piston controller, with six configurable pistons, optional two-piston heads and separate shows.

Open the desired `.ino` in Arduino IDE. Each sketch lives in a matching folder so Arduino builds them independently.

## Actuator configuration

Edit these headers inside `MandoTroneresActuadors`:

- `enviroment_pins.h`: output pins and maximum motor travel times in milliseconds.
- `enviroment_dmx.h`: DMX start address, channel count and channel assignments.
- `configuration.h`: DMX activation threshold, movement command ranges, timeline polling interval and shutter button pulse duration.

Runtime state stays with the code that owns it. Show event times and individual movement durations remain in the actuator sketch, alongside their actions.

The actuator sketch requires TaskScheduler (the existing source targets v4.0.3 with LTS enabled) and Conceptinetics. Select the board matching the installed hardware; the current pin assignments use pins up to 53.

## Piston controller

The piston sketch uses TaskScheduler and Conceptinetics, like the actuator sketch.

- `MandoTroneresPistons/enviroment_pins.h`: assign each piston output pin. `-1` means unassigned; no GPIO is written for that piston.
- `MandoTroneresPistons/enviroment_dmx.h`: assign the DMX start address and relative channels (1-based). `-1` means unassigned. DMX stays disabled until a valid start address is supplied.
- `MandoTroneresPistons/configuration.h`: set each piston's open and close times in milliseconds. Both must be nonzero to enable auto movement. They start at zero pending hardware configuration.
- `MandoTroneresPistons/Shows/`: edit `show.h`, `preshow.h` and `exampleShow.h`. Shows can access pistons, heads and variables declared before their includes in the main sketch. Initial timelines contain no hardware movements; preshow is empty.

### Piston API

```cpp
piston1.setState(PistonState::Open);   // HIGH, maintained until changed
piston1.setState(PistonState::Closed); // LOW, maintained until changed
piston1.changeState();                // Toggle
PistonState state = piston1.getState();
piston1.startAutoMovement();
```

One auto request opens the piston, waits its open time, closes it, then waits its close time before accepting another cycle. Include travel and any desired hold time in these durations. Repeated requests while busy do not restart the timer. Dropping the DMX auto signal lets the current cycle finish; holding it repeats cycles after completion. Explicit `setState`/`changeState` commands cancel an active cycle. State getters report the commanded output, without position sensors. Outputs initialize closed from `setup()`.

### Head API

```cpp
head1.setState(HeadState::Left);   // Both closed
head1.setState(HeadState::Middle); // First open, second closed
head1.setState(HeadState::Right);  // Both open
HeadState state = head1.getState();
```

The main sketch provides optional pairings: head1 uses pistons 1/2, head2 uses 3/4, and head3 uses 5/6. Change these references as needed. Constructing a head does not reserve its pistons or require assigning a DMX channel. Both outputs change in the same loop pass when positioning a head. If a piston is controlled individually, either mixed combination reads back as middle; head commands always use first-open/second-closed for middle.

### DMX behavior

| Command | Values |
| --- | --- |
| Show / preshow / example show | Above 192 activates; same priority order as the actuator sketch |
| Piston state channel | 0: no command; 1–127: close; 128–255: open |
| Piston auto channel | Above 192 requests a cycle, repeated while held |
| Head state channel | 0: no command; 1–85: left; 86–170: middle; 171–255: right |

Piston state commands override auto requests. Active head commands override individual commands for their two pistons. Leave direct channels unassigned for head-only pistons. With no command, outputs retain their state and active auto cycles finish. While a show timeline runs, manual DMX commands are not processed, but scheduler-driven piston cycles keep running. Held show signals restart their timeline after completion, matching the actuator controller.

## Branches

`2026` preserves the original final version at commit `8049278`. Continue development on `main`.

To freeze `2026` on GitHub, open repository Settings > Rules > Rulesets > New branch ruleset. Set enforcement to Active, target the exact branch `2026`, and enable Restrict updates, Restrict deletions, and Block force pushes. Leave the bypass list empty to prevent bypass updates. Availability depends on repository visibility and GitHub plan. See [GitHub's ruleset documentation](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/managing-rulesets/creating-rulesets-for-a-repository).

GitHub protections apply to remote updates; local Git can still create commits on any checked-out branch. Archiving the entire repository would also freeze development on `main`, so use branch protection for this archive branch.
