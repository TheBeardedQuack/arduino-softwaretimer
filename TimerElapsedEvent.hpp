#ifndef TBQ_TIMERELAPSEDEVENT_HPP
#define TBQ_TIMERELAPSEDEVENT_HPP

#include "TimerTypeDefs.hpp"

namespace TBQ
{
    namespace Timers
    {
        class TimerElapsedEventArgs;
        typedef void(*TimerElapsedEventHandler)(TimerElapsedEventArgs);

        
        class TimerElapsedEventArgs
        {
        public:
            TimerElapsedEventArgs(
                HndTimer handle,
                void* userObject = nullptr
                );
            
            TimerElapsedEventArgs(
                const TimerElapsedEventArgs& copy
                ) = default;
            
            TimerElapsedEventArgs(
                TimerElapsedEventArgs&& move
                ) = default;
            
            TimerElapsedEventArgs&
            operator=(
                const TimerElapsedEventArgs& copy
                ) = default;

            const HndTimer hndTimer;
            void* const userObject;
        };
    }
}

#endif
