#ifndef KROMBLAST_LISTENER_HPP
#define KROMBLAST_LISTENER_HPP

#include <string>
#include <functional>
#include <vector>
#include <map>
#include "kb_lib_signal.hpp"

namespace Kromblast
{
    class Dispatcher : public KromblastCore::Signal::Dispatcher
    {
    private:
        std::map<std::string, std::vector<KromblastCore::Signal::SignalHandler *>> listeners;

    public:
        Dispatcher();
        ~Dispatcher();

        void listen(std::string channel, KromblastCore::Signal::SignalHandler *handler);

        void dispatch(KromblastCore::Signal::Signal signal);
        void dispatch(std::string channel, std::string message);
    };
}

#endif