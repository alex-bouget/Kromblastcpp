#ifndef KRBLST_API_SIGNAL_H
#define KRBLST_API_SIGNAL_H

#include <string>

namespace Kromblast
{
    namespace Api
    {
        typedef struct Signal Signal;

        struct Signal
        {
            std::string channel;
            std::string message;
        };

        class SignalHandlerInterface
        {
        public:
            virtual void handle(Signal signal) = 0;
        };

        class DispatcherInterface
        {
        public:
            virtual ~DispatcherInterface() {}
            virtual void listen(std::string channel, SignalHandlerInterface *handler) = 0;

            virtual void dispatch(Signal signal) = 0;
            virtual void dispatch(std::string channel, std::string message) = 0;
        };

    }
}

#endif