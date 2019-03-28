#include "TimerElapsedEvent.hpp"

namespace TBQ
{
    namespace Timers
    {
        TimerElapsedEventArgs::TimerElapsedEventArgs(
            HndTimer hndTimer,
            void* userObject = nullptr
        ) :
            hndTimer(hndTimer),
            userObject(userObject)
        {
            // Nothing to do
        }
    }
}
