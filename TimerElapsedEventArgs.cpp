#include "TimerElapsedEventArgs.hpp"

TimerElapsedEventArgs(
    HndTimer hndTimer,
    void* userObject = nullptr
) :
    hndTimer(hndTimer),
    userObject(userObject)
{
    // Nothing to do
}
