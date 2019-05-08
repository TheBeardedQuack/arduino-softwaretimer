# Arduino Software Timer

This library is targeted at Arduino based SBCs and provides a simple mechanism for setting up software timers. It is expected in the main loop that the `ServiceTimers()` function is called in order for this library to work as intended. The `ServiceTimers` functions is responsible for calling the timer event handlers as well as freeing the timers after use.

## Example Usage
### Fire and forget

```cpp
#include "Timer.hpp"

const int OutputPin = 13;

void OnTimerElapsed(
    TBQ::Timers::TimerElapsedEventArgs // Event parameters
) {
    digitalWrite(OutputPin, LOW);
}

void setup()
{
    pinMode(OutputPin, OUTPUT);
    digitalWrite(OutputPin, HIGH);

    //Create timer
    auto tmr = TBQ::Timers::StartTimer(
        5000, // Timer delay in milliseconds
        false, // Auto restart timer after elapsed?
        OnTimerElapsed // Event handler
    );

    if(!tmr)
    {
        // Failed to start timer.
        // `tmr.ErrorCode` contains the fail reason
    }
    else
    {
        // Timer handle is found at `tmr.Result`
        // For this example we don't need to remember the handle
    }
}

void loop()
{
    TBQ::Timers::ServiceTimers();

    // Do rest of program loop
}
```

In the example above we have turned on a digital pin in our setup function and then created a "single-shot" timer with a delay of 5000ms.
In our main loop we call `ServiceTimers()` which allows the library to work its magic.

After the 5000ms has elapsed, the next time `ServiceTimers()` is called it will internally call our event handler and then automatically cleanup the timer resources (as our timer is not setup to auto-restart).

### Periodic timer

```cpp
#include "Timer.hpp"

const int OutputPin = 13;
bool pinActive = false;

void OnTimerElapsed(
    TBQ::Timers::TimerElapsedEventArgs // Event parameters
) {
    if(pinActive)
    {
        digitalWrite(OutputPin, LOW);
        pinActive = false;
    }
    else
    {
        digitalWrite(OutputPin, HIGH);
        pinActive = true;
    }
}

void setup()
{
    pinMode(OutputPin, OUTPUT);

    //Create timer
    auto tmr = TBQ::Timers::StartTimer(
        5000, // Timer delay in milliseconds
        true, // Auto restart timer after elapsed?
        OnTimerElapsed // Event handler
    );

    if(!tmr)
    {
        // Failed to start timer.
        // `tmr.ErrorCode` contains the fail reason
    }
    else
    {
        // Timer handle is found at `tmr.Result`
        // For this example we don't need to remember the handle
    }
}

void loop()
{
    TBQ::Timers::ServiceTimers();

    // Do rest of program loop
}
```

### Passing parameters to the event handler

```cpp
//TODO: UserObject example
```

### Remarks about timer lifecycle
Once a timer has been created with `StartTimer()` it remains valid until one of the following is met:
+ `StopTimer()` is called with the desired timer handle.
+ The timer callback has returned and `autoReset` is false.

Timer handles will always be valid during the callback execution (unless `StopTimer()` is called during the callback).

Timers are only automatically freed during the `ServiceTimers()` function call. This means that a you only need to check a timer is valid once before use (unless `StopTimer()` is explicitely called from your program) and the timer is guarenteed to be valid until the next call to `ServiceTimers()`.
