#pragma once

TimelineEvent exampleShow[] = {
    {0, [] { showActive = true; }},
    // Example: {1000, [] { dorsal.startAutoMovement(); }},
    {2000, [] { showActive = false; }}
};
