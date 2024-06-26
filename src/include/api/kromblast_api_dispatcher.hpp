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
            virtual void listen(const std::string &channel, SignalHandlerInterface *handler) = 0;
            virtual void listen(SignalHandlerInterface *handler) = 0;

            virtual void dispatch(const Signal &signal) = 0;
            virtual void dispatch(const std::string &channel, const std::string &message) = 0;
        };

    }
}

#endif