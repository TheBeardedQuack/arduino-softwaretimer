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

        ErrorReturn<Timer*> GetActiveTimer(HndTimer hndTimer){
            if(hndTimer == nullptr)
                return {EError::InvalidHandle, nullptr};

            for(int i=0; i<TBQ_TIMER_COUNT; i++)
            {
                if(hndTimer == &sTimers[i]){
                    if(sTimers[i].inUse)
                        return {EError::NoError, (Timer*)hndTimer};
                    return {EError::InvalidHandle, nullptr};
                }
            }
            return {EError::InvalidHandle, nullptr};
        }

        void
        ServiceTimers()
        {
            auto timeNow = millis();

            for(int i=0; i<TBQ_TIMER_COUNT; i++)
            {
                if(!sTimers[i].inUse)
                    continue;
                
                auto pTimer = &sTimers[i];
                if(pTimer->duration < timeNow - pTimer->start)
                {
                    pTimer->callback({pTimer, pTimer->userObject});

                    //Check timer has not been reset during callback
                    if(pTimer->duration < timeNow - pTimer->start)
                    {
                        if(pTimer->autoReset)
                            pTimer->start = timeNow;
                        else
                            pTimer->inUse = false;
                    }
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
                return {EError::NoError, &sTimers[i]};
            }

            return {EError::OutOfResources, nullptr};
        }

        ErrorReturn<Tick>
        GetDuration(
            HndTimer handle
        ){
            auto getTimer = GetActiveTimer(handle);
            if(!getTimer)
                return {getTimer.ErrorCode, 0};
            
            return {EError::NoError, getTimer.Result->duration};
        }

        ErrorReturn<Tick>
        GetTimeRemaining(
            HndTimer handle
        ){
            auto getTimer = GetActiveTimer(handle);
            if(!getTimer)
                return {getTimer.ErrorCode, 0};

            return {EError::NoError, 
                getTimer.Result->duration - (millis() - getTimer.Result->start)};
        }

        ErrorReturn<Tick>
        GetTimeElapsed(
            HndTimer handle
        ){
            auto getTimer = GetActiveTimer(handle);
            if(!getTimer)
                return {getTimer.ErrorCode, 0};

            return {EError::NoError, millis() - getTimer.Result->start};
        }

        ErrorReturn<bool>
        GetAutoReset(
            HndTimer handle
        ){
            auto getTimer = GetActiveTimer(handle);
            if(!getTimer)
                return {getTimer.ErrorCode, false};

            return {EError::NoError, getTimer.Result->autoReset};
        }

        EError
        SetDuration(
            HndTimer handle,
            Tick duration,
            bool resetNow = false
        ){
            auto getTimer = GetActiveTimer(handle);
            if(!getTimer)
                return getTimer.ErrorCode;
            
            if(duration > TBQ_TIMER_MAXDURATION)
                return EError::InvalidInterval;

            getTimer.Result->duration = duration;
            if(resetNow)
                getTimer.Result->start = millis();

            return EError::NoError;
        }

        EError
        SetAutoReset(
            HndTimer handle, 
            bool autoReset
        ){
            auto getTimer = GetActiveTimer(handle);
            if(!getTimer)
                return getTimer.ErrorCode;
            
            getTimer.Result->autoReset = autoReset;
            return EError::NoError;
        }

        EError
        SetUserObject(
            HndTimer handle,
            void* userObject
        ){
            auto getTimer = GetActiveTimer(handle);
            if(!getTimer)
                return getTimer.ErrorCode;
            
            getTimer.Result->userObject = userObject;
            return EError::NoError;
        }

        void
        StopTimer(
            HndTimer handle
        ){
            auto getTimer = GetActiveTimer(handle);
            if(!getTimer)
                return;
            
            getTimer.Result->inUse = false;
        }

        EError
        ResetTimer(
            HndTimer handle
        ){
            auto getTimer = GetActiveTimer(handle);
            if(!getTimer)
                return getTimer.ErrorCode;

            getTimer.Result->start = millis();
        }
    
    } //Namespace Timers
}//Namespace TBQ
