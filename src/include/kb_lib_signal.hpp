#ifndef KB_LIB_SIGNAL_HPP
#define KB_LIB_SIGNAL_HPP

#include <string>

namespace KromblastCore
{
    namespace Signal
    {
        typedef struct Signal Signal;

        struct Signal
        {
            std::string channel;
            std::string message;
        };

        class SignalHandler
        {
        public:
            virtual void handle(Signal signal) = 0;
        };

        class Dispatcher
        {
        public:
            virtual void listen(std::string channel, SignalHandler *handler) = 0;

            virtual void dispatch(Signal signal) = 0;
            virtual void dispatch(std::string channel, std::string message) = 0;
        };

    }
}

#endif