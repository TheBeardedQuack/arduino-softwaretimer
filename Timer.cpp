#include "Timer.hpp"

namespace TBQ{
    namespace Timers{
                
        struct Timer {
            Tick start;
            Tick duration;
            bool inUse;
            bool autoReset;
            TimerElapsedEventHandler callback;
            void* userObject;
        };
        
        Timer sTimers[TBQ_TIMER_COUNT];

        void
        ServiceTimers()
        {
            auto timeNow = millis();

            for(int i=0; i<TBQ_TIMER_COUNT; i++){
                if(!sTimers[i].inUse)
                    continue;
                
                auto pTimer = &sTimers[i];
                if(pTimer->duration < timeNow - pTimer->start){
                    pTimer->callback({pTimer, pTimer->userObject});
                    if(pTimer->autoReset)
                        pTimer->start = timeNow;
                    else
                        pTimer->inUse = false;
                }
            }
        }

        ErrorReturn<HndTimer>
        StartTimer(
            Tick duration,
            bool autoReset,
            TimerElapsedEventHandler callback,
            void* userObject
        ){
            if(duration > TBQ_TIMER_MAXDURATION)
                return {EError::InvalidInterval, nullptr};
            if(callback == nullptr)
                return {EError::InvalidCallback, nullptr};

            for(int i=0; i<TBQ_TIMER_COUNT; i++){
                if(sTimers[i].inUse)
                    continue;
                
                sTimers[i] = {
                    millis(),
                    duration,
                    true,
                    autoReset,
                    callback,
                    userObject
                };
                return {EError::Success, &sTimers[i]};
            }

            return {EError::OutOfResources, nullptr};
        }

        ErrorReturn<Tick>
        GetDuration(
            HndTimer handle
        ){
            if(handle == nullptr)
                return {EError::InvalidHandle, 0};
            
            auto pTimer = (Timer*)handle;
            if(!(pTimer->inUse))
                return {EError::InvalidHandle, 0};

            return {EError::Success, pTimer->duration};
        }

        ErrorReturn<Tick>
        GetTimeRemaining(
            HndTimer handle
        ){
            if(handle == nullptr)
                return {EError::InvalidHandle, 0};
            
            auto pTimer = (Timer*)handle;
            if(!(pTimer->inUse))
                return {EError::InvalidHandle, 0};


            return {EError::Success, pTimer->duration}
        }

        ErrorReturn<Tick>
        GetTimeElapsed(
            HndTimer handle
        ){
            if(handle == nullptr)
                return {EError::InvalidHandle, 0};
            
            auto pTimer = (Timer*)handle;
            if(!(pTimer->inUse))
                return {EError::InvalidHandle, 0};

            return {EError::Success, millis() - pTimer->start};
        }

        ErrorReturn<bool>
        GetAutoReset(
            HndTimer handle
        ){
            if(handle == nullptr)
                return {EError::InvalidHandle, 0};
            
            auto pTimer = (Timer*)handle;
            if(!(pTimer->inUse))
                return {EError::InvalidHandle, 0};

            return {EError::Success, pTimer->autoReset};
        }

        EError SetDuration(
            HndTimer handle,
            Tick duration,
            bool resetNow = false
        ){
            if(handle == nullptr)
                return EError::InvalidHandle;
            
            auto pTimer = (Timer*)handle;
            if(!(pTimer->inUse))
                return EError::InvalidHandle;

            if(duration > TBQ_TIMER_MAXDURATION)
                return EError::InvalidInterval;

            pTimer->duration = duration;
            if(resetNow)
                pTimer->start = millis();

            return EError::Success;
        }

        EError
        SetAutoReset(
            HndTimer handle, 
            bool autoReset
        ){
            if(handle == nullptr)
                return EError::InvalidHandle;
            
            auto pTimer = (Timer*)handle;
            if(!(pTimer->inUse))
                return EError::InvalidHandle;

            pTimer->autoReset = autoReset;

            return EError::Success;
        }

        EError
        SetUserObject(
            HndTimer handle,
            void* userObject
        ){
            if(handle == nullptr)
                return EError::InvalidHandle;
            
            auto pTimer = (Timer*)handle;
            if(!(pTimer->inUse))
                return EError::InvalidHandle;

            pTimer->userObject = userObject;

            return EError::Success;
        }

        EError
        StopTimer(
            HndTimer handle
        ){
            if(handle == nullptr)
                return EError::InvalidHandle;
            
            // Don't check if in use if we're stopping anyway
            // Allow repeated cleanup without punishment
            auto pTimer = (Timer*)handle;
            pTimer->inUse = false;

            return EError::Success;
        }
    
    } //Namespace Timers
}//Namespace TBQ
