#pragma once

// Add timed actions here; pistons and heads are declared before this include.
TimelineEvent mainShow[] = {
    {0, [] { showActive = true; }},
    // Example: {1000, [] { head.setState(HeadState::Center); }},
    {2000, [] { showActive = false; }}
};
