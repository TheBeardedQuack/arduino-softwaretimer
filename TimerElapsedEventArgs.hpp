#ifndef TBQ_TIMERELAPSEDEVENTARGS_HPP
#define TBQ_TIMERELAPSEDEVENTARGS_HPP

namespace TBQ
{
    namespace Timers
    {
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
