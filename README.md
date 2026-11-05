# MandoTroneres

Two independent Arduino sketches:

- `MandoTroneresActuadors/MandoTroneresActuadors.ino`: the existing actuator controller, with its show timelines and DMX behavior preserved.
- `MandoTroneresPistons/MandoTroneresPistons.ino`: an empty starting sketch for the piston controller. It does not configure or drive hardware yet.

Open the desired `.ino` in Arduino IDE. Each sketch lives in a matching folder so Arduino builds them independently.

## Actuator configuration

Edit these headers inside `MandoTroneresActuadors`:

- `enviroment_pins.h`: output pins and maximum motor travel times in milliseconds.
- `enviroment_dmx.h`: DMX start address, channel count and channel assignments.
- `configuration.h`: DMX activation threshold, movement command ranges, timeline polling interval and shutter button pulse duration.

Runtime state stays with the code that owns it. Show event times and individual movement durations remain in the actuator sketch, alongside their actions.

The actuator sketch requires TaskScheduler (the existing source targets v4.0.3 with LTS enabled) and Conceptinetics. Select the board matching the installed hardware; the current pin assignments use pins up to 53.

## Branches

`2026` preserves the original final version at commit `8049278`. Continue development on `main`.

To freeze `2026` on GitHub, open repository Settings > Rules > Rulesets > New branch ruleset. Set enforcement to Active, target the exact branch `2026`, and enable Restrict updates, Restrict deletions, and Block force pushes. Leave the bypass list empty to prevent bypass updates. Availability depends on repository visibility and GitHub plan. See [GitHub's ruleset documentation](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/managing-rulesets/creating-rulesets-for-a-repository).

GitHub protections apply to remote updates; local Git can still create commits on any checked-out branch. Archiving the entire repository would also freeze development on `main`, so use branch protection for this archive branch.
