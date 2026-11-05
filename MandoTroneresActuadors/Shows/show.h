#pragma once

// Included after the timeline type and motor declarations in the sketch.
TimelineEvent mainShow[] = {
    {0, [] { showActive = true; }},
    {1000, [] {}},
    {2000, [] { showActive = false; }}
};
