#pragma once

// Included after the timeline type and motor declarations in the sketch.
TimelineEvent exampleShow[] = {
    {0, []{showActive = true;}},
    {2500,   [] {}},
    {4500,  [] { showActive = false; }}};
