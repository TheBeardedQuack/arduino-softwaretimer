#ifndef TBQ_TIMER_HPP
#define TBQ_TIMER_HPP

#include "TBQ.hpp"
#include "Error.hpp"

#ifndef TBQ_TIMER_COUNT
#define TBQ_TIMER_COUNT 10
#endif

//Max timer allowed is 1 day (in milliseconds)
#define TBQ_TIMER_MAXDURATION 86400000

#if TBQ_TIMER_COUNT == 0
#error TBQ_TIMER_COUNT cannot be 0, either increase the timer count or unload the module.
#endif

namespace TBQ
{
    namespace Timers
    {
        typedef unsigned long Tick;

        typedef Handle HndTimer;

        typedef struct {
            HndTimer hndTimer;
            void* userObject = nullptr;
        } TimerElapsedEventArgs;

        typedef void(*TimerElapsedEventHandler)(TimerElapsedEventArgs);

        void
        ServiceTimers();

        ErrorReturn<HndTimer>
        StartTimer(
            Tick duration,
            bool autoReset,
            TimerElapsedEventHandler callback,
            void* userObject = nullptr
            );

        ErrorReturn<Tick>
        GetDuration(
            HndTimer handle
            );

        ErrorReturn<Tick>
        GetTimeRemaining(
            HndTimer handle
            );

        ErrorReturn<Tick>
        GetTimeElapsed(
            HndTimer handle
            );

        ErrorReturn<bool>
        GetAutoReset(
            HndTimer handle
            );

        EError SetDuration(
            HndTimer handle,
            Tick duration,
            bool resetNow = false
            );

        EError
        SetAutoReset(
            HndTimer handle, 
            bool autoReset
            );

        EError
        SetUserObject(
            HndTimer handle,
            void* userObject
            );

        void
        StopTimer(
            HndTimer handle
            );
        
        EError
        ResetTimer(
            HndTimer handle
            );
    }
}

#endif
