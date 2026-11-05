#pragma once

// Restore the preshow position whenever the preshow DMX command is active.
inline void runPreshow()
{
  mouth.goDownMax();
}
