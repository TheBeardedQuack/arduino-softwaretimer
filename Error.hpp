#ifndef TBQ_ERROR_HPP
#define TBQ_ERROR_HPP

#include "TBQ.hpp"

#define TBQ_ERRCAT_GENERIC 0x000
#define TBQ_ERRCAT_SYSLIMITS 0x100
#define TBQ_ERRCAT_PARAMETER 0x200

#define TBQ_ERRSRC_GENGIC 0x0000
#define TBQ_ERRSRC_TIMER 0x1000

namespace TBQ{

    typedef unsigned long ErrorCode;

    enum struct EError : ErrorCode
    {
        NoError = 0,

        /*** GENERIC SYSTEM ERROR CODES ***/

        // System limits
        OutOfResources = TBQ_ERRSRC_GENGIC + TBQ_ERRCAT_SYSLIMITS,

        // Parameter Errors
        InvalidParameter = TBQ_ERRSRC_GENGIC + TBQ_ERRCAT_PARAMETER,
        InvalidHandle,

        /*** TIMER ERROR CODES ***/

        // Good responses

        // System Limits

        // Parameter Errors
        InvalidInterval = TBQ_ERRSRC_TIMER + TBQ_ERRCAT_PARAMETER,
        InvalidCallback,
    };

    template<class TResult> 
    struct ErrorReturn
    {
        const EError ErrorCode;
        T Result;

        operator bool() const { return ErrorCode == EError::Success; }
        bool operator!() const {return !operator bool; }
    };

} //Namespace TBQ

#endif
