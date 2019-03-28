#include "Timer.hpp"
#include <Arduino.h>

namespace TBQ
{
    namespace Timers
    {
                
        struct Timer
        {
            Tick start;
            Tick duration;
            bool inUse;
            bool autoReset;
            TimerElapsedEventHandler callback;
            void* userObject;
        };
        
        Timer sTimers[TBQ_TIMER_COUNT];

        ErrorReturn<Timer*>
        GetActiveTimer(
            HndTimer hndTimer
        ){
            if(hndTimer == nullptr)
            {
                return ErrorReturn<Timer*>{
                    EError::InvalidHandle,
                    nullptr
                    };
            }

            for(int i=0; i<TBQ_TIMER_COUNT; i++)
            {
                if(hndTimer == &sTimers[i]){
                    if(sTimers[i].inUse)
                    {
                        return ErrorReturn<Timer*>{
                            EError::NoError,
                            (Timer*)hndTimer
                            }; 
                    }
                    return ErrorReturn<Timer*>{
                        EError::InvalidHandle,
                        nullptr
                        };
                }
            }
            return ErrorReturn<Timer*>{
                EError::InvalidHandle,
                nullptr
                };
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
                    pTimer->callback(TimerElapsedEventArgs{(HndTimer)pTimer, pTimer->userObject});

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
            {
                return EErrorReturn<HndTimer>{
                    EError::InvalidInterval,
                    nullptr
                    };
            }
            if(callback == nullptr)
            {
                return EErrorReturn<HndTimer>{
                    EError::InvalidCallback,
                    nullptr
                    };
            }

            for(int i=0; i<TBQ_TIMER_COUNT; i++){
                if(sTimers[i].inUse)
                    continue;
                
                sTimers[i] = Timer{
                    millis(),
                    duration,
                    true,
                    autoReset,
                    callback,
                    userObject
                };
                return EErrorReturn<HndTimer>{
                    EError::NoError,
                    &sTimers[i]
                    };
            }

            return EErrorReturn<HndTimer>{
                EError::OutOfResources,
                nullptr
                };
        }

        ErrorReturn<Tick>
        GetDuration(
            HndTimer handle
        ){
            auto getTimer = GetActiveTimer(handle);
            if(!getTimer)
            {
                return EErrorReturn<Tick>{
                    getTimer.ErrorCode,
                    0
                    };
            }
            
            return EErrorReturn<Tick>{
                EError::NoError,
                getTimer.Result->duration
                };
        }

        ErrorReturn<Tick>
        GetTimeRemaining(
            HndTimer handle
        ){
            auto getTimer = GetActiveTimer(handle);
            if(!getTimer)
            {
                return EErrorReturn<Tick>{
                    getTimer.ErrorCode,
                    0
                    }; 
            }

            return EErrorReturn<Tick>{
                EError::NoError,
                getTimer.Result->duration - (millis() - getTimer.Result->start)
                };
        }

        ErrorReturn<Tick>
        GetTimeElapsed(
            HndTimer handle
        ){
            auto getTimer = GetActiveTimer(handle);
            if(!getTimer)
            {
                return EErrorReturn<Tick>{
                    getTimer.ErrorCode,
                    0
                    };
            }

            return EErrorReturn<Tick>{
                EError::NoError,
                millis() - getTimer.Result->start
                };
        }

        ErrorReturn<bool>
        GetAutoReset(
            HndTimer handle
        ){
            auto getTimer = GetActiveTimer(handle);
            if(!getTimer){
                return EErrorReturn<bool>{
                    getTimer.ErrorCode,
                    false
                    };
            }

            return EErrorReturn<bool>{
                EError::NoError,
                getTimer.Result->autoReset
                };
        }

        EError
        SetDuration(
            HndTimer handle,
            Tick duration,
            bool resetNow
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
